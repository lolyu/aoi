/*
 * list_interfaces.c
 *
 * Minimal demo: connect to VPP via its Unix API socket and print all
 * interfaces using the same M / S / W / PING macro pattern used in
 * SaiVppXlate.c.
 *
 * Build:
 *   make -f Makefile.list_interfaces
 *
 * Run (VPP must be running):
 *   sudo ./list_interfaces
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>

#include <vat/vat.h>
#include <vlibapi/api.h>
#include <vlibmemory/api.h>
#include <vppinfra/error.h>
#include <vlibapi/vat_helper_macros.h>

/* ── interface plugin ──────────────────────────────────────────────── */
#include <vnet/interface.api_enum.h>
#include <vnet/interface.api_types.h>

#define vl_typedefs
#include <vnet/interface.api.h>
#undef vl_typedefs

#define vl_endianfun
#include <vnet/interface.api.h>
#undef vl_endianfun

#define vl_calcsizefun
#include <vnet/interface.api.h>
#undef vl_calcsizefun

#define vl_api_version(n, v) static u32 interface_api_version = v;
#include <vnet/interface.api.h>
#undef vl_api_version

/* ── memclnt (control_ping lives here) ─────────────────────────────── */
#include <vlibmemory/memclnt.api_enum.h>

#define vl_typedefs
#include <vlibmemory/memclnt.api.h>
#undef vl_typedefs

#define vl_endianfun
#include <vlibmemory/memclnt.api.h>
#undef vl_endianfun

#define vl_calcsizefun
#include <vlibmemory/memclnt.api.h>
#undef vl_calcsizefun

/* ── VPE base (get_first_msg_id_reply) ─────────────────────────────── */
#include <vnet/format_fns.h>

/* ─────────────────────────────────────────────────────────────────────
 * Required VPP runtime stubs
 * ───────────────────────────────────────────────────────────────────── */
void classify_get_trace_chain(void) {}
void os_exit(int code) { exit(code); }

/* vat_main is a global used by the M / S / W / PING macros */
vat_main_t vat_main;

/* interface_name → sw_if_index (populated by the reply handler) */
static uword *interface_name_by_sw_index = NULL;

f64 vat_time_now(vat_main_t *vam)
{
    return clib_time_now(&vam->clib_time);
}

void __clib_no_tail_calls
vat_suspend(vlib_main_t *vm, f64 interval)
{
    const struct timespec req = { 0, 100000000 }; /* 100 ms */
    nanosleep(&req, NULL);
}

void vl_noop_handler(void *mp) {}

/* ─────────────────────────────────────────────────────────────────────
 * set_reply_status — called by every reply handler to unblock W()
 * ───────────────────────────────────────────────────────────────────── */
static void set_reply_status(int retval)
{
    vat_main_t *vam = &vat_main;
    vam->retval      = retval;
    vam->result_ready = 1;
}

/* ─────────────────────────────────────────────────────────────────────
 * Reply handlers
 * ───────────────────────────────────────────────────────────────────── */

/*
 * Called once per interface by VPP in response to SW_INTERFACE_DUMP.
 * Stores name ↔ sw_if_index mappings for later lookup.
 */
static void
vl_api_sw_interface_details_t_handler(vl_api_sw_interface_details_t *mp)
{
    vat_main_t *vam = &vat_main;

    /* format() allocates a null-terminated copy of the interface name */
    u8 *s = format(0, "%s%c", mp->interface_name, 0);

    /* name → index */
    hash_set_mem(vam->sw_if_index_by_interface_name, s,
                 ntohl(mp->sw_if_index));

    /* index → name */
    hash_set(interface_name_by_sw_index, ntohl(mp->sw_if_index), s);
}

/*
 * Called after all sw_interface_details have been sent by VPP.
 * Setting result_ready unblocks the W() macro in dump_interfaces().
 */
static void
vl_api_control_ping_reply_t_handler(vl_api_control_ping_reply_t *mp)
{
    vat_main_t *vam = &vat_main;

    set_reply_status(ntohl(mp->retval));

    if (vam->socket_client_main)
        vam->socket_client_main->control_pings_outstanding--;
}

/*
 * VPP's get_first_msg_id_reply is needed so we can look up the dynamic
 * plugin message ID base for the "interface" plugin.
 */
static void
vl_api_get_first_msg_id_reply_t_handler(vl_api_get_first_msg_id_reply_t *mp)
{
    /* handled internally by vl_client_get_first_plugin_msg_id() */
}

/* ─────────────────────────────────────────────────────────────────────
 * Message ID bases
 * ───────────────────────────────────────────────────────────────────── */
static u16 interface_msg_id_base;
static u16 memclnt_msg_id_base;
static u16 __plugin_msg_base; /* used by the M() macro */

/* ─────────────────────────────────────────────────────────────────────
 * Handler registration
 *
 * Two sets:
 *   base  — fixed-ID messages: control_ping_reply, get_first_msg_id_reply
 *   ext   — plugin messages with dynamic ID base: sw_interface_details
 * ───────────────────────────────────────────────────────────────────── */

/*
 * vl_msg_api_set_handlers wrapper — keeps the signature compatible with
 * older VPP builds where the upstream function has a different arity.
 */
static void
vl_msg_api_set_handlers(int id, char *name,
                        void *handler, void *cleanup, void *endian,
                        int size, int traced,
                        void *tojson, void *fromjson, void *calc_size)
{
    vl_msg_api_msg_config_t cfg;
    clib_memset(&cfg, 0, sizeof(cfg));
    cfg.id          = id;
    cfg.name        = name;
    cfg.handler     = handler;
    cfg.cleanup     = cleanup;
    cfg.endian      = endian;
    cfg.traced      = traced;
    cfg.replay      = 1;
    cfg.tojson      = tojson;
    cfg.fromjson    = fromjson;
    cfg.calc_size   = calc_size;
    vl_msg_api_config(&cfg);
}

#define MEMCLNT_MSG_ID(id)   VL_API_##id
#define INTERFACE_MSG_ID(id) (VL_API_##id + interface_msg_id_base)

/* Base messages (fixed IDs from the memclnt range) */
#define foreach_base_reply_msg                                          \
    _(MEMCLNT_MSG_ID(GET_FIRST_MSG_ID_REPLY), get_first_msg_id_reply)  \
    _(MEMCLNT_MSG_ID(CONTROL_PING_REPLY),     control_ping_reply)

static void register_base_handlers(void)
{
#define _(N, n)                                         \
    vl_msg_api_set_handlers(                            \
        N + 1, #n,                                      \
        vl_api_##n##_t_handler,                         \
        vl_noop_handler,                                \
        vl_api_##n##_t_endian,                          \
        sizeof(vl_api_##n##_t), 1,                      \
        vl_noop_handler,                                \
        vl_noop_handler,                                \
        vl_api_##n##_t_calc_size);
    foreach_base_reply_msg;
#undef _
}

/* Interface plugin messages (dynamic IDs, require interface_msg_id_base) */
#define foreach_interface_reply_msg                                         \
    _(INTERFACE_MSG_ID(SW_INTERFACE_DETAILS), sw_interface_details)

static void register_interface_handlers(void)
{
#define _(N, n)                                         \
    vl_msg_api_set_handlers(                            \
        N, #n,                                          \
        vl_api_##n##_t_handler,                         \
        vl_noop_handler,                                \
        vl_api_##n##_t_endian,                          \
        sizeof(vl_api_##n##_t), 1,                      \
        vl_noop_handler,                                \
        vl_noop_handler,                                \
        vl_api_##n##_t_calc_size);
    foreach_interface_reply_msg;
#undef _
}

/* ─────────────────────────────────────────────────────────────────────
 * Resolve dynamic plugin message ID bases from VPP
 * ───────────────────────────────────────────────────────────────────── */
static void get_msg_id_bases(void)
{
    u8 *name;

    name = format(0, "interface_%08x%c", interface_api_version, 0);
    interface_msg_id_base =
        vl_client_get_first_plugin_msg_id((char *) name);
    assert(interface_msg_id_base != (u16) ~0);
    vec_free(name);

    memclnt_msg_id_base = 0; /* memclnt messages always start at 0 */
}

/* ─────────────────────────────────────────────────────────────────────
 * Connect to VPP
 * ───────────────────────────────────────────────────────────────────── */
#define API_SOCKET_FILE "/run/vpp/api.sock"

static int connect_to_vpp(const char *client_name)
{
    vat_main_t *vam  = &vat_main;
    api_main_t *am   = vlibapi_get_main();

    vam->socket_client_main = &socket_client_main;

    if (vl_socket_client_connect((char *) vam->socket_name,
                                 (char *) client_name,
                                 0 /* default rx/tx buffers */))
        return -1;

    vam->my_client_index = htonl(socket_client_main.client_index);
    am->my_client_index  = vam->my_client_index;
    return 0;
}

/* ─────────────────────────────────────────────────────────────────────
 * Initialization
 * ───────────────────────────────────────────────────────────────────── */
static int init(void)
{
    vat_main_t *vam = &vat_main;

    clib_mem_init(0, 64 << 20); /* 64 MB arena */
    vlib_main_init();
    clib_time_init(&vam->clib_time);

    /* Reserve message IDs above the built-in memclnt range */
    vl_msg_api_set_first_available_msg_id(VL_MSG_MEMCLNT_LAST + 1);

    /* Register base handlers (control_ping_reply, get_first_msg_id_reply) */
    register_base_handlers();

    /* Build the socket name string */
    vam->socket_name =
        format(0, "%s%c", API_SOCKET_FILE, 0);

    /* Hash tables for name ↔ sw_if_index lookups */
    vam->sw_if_index_by_interface_name =
        hash_create_string(0, sizeof(uword));
    interface_name_by_sw_index =
        hash_create(0, sizeof(uword));

    if (connect_to_vpp("list_interfaces_demo") != 0) {
        fprintf(stderr, "ERROR: cannot connect to VPP at %s\n"
                        "       Is VPP running? (sudo systemctl status vpp)\n",
                API_SOCKET_FILE);
        return -1;
    }

    /* Now we can talk to VPP — resolve dynamic plugin message IDs */
    get_msg_id_bases();

    /* Register interface-plugin handlers (needs interface_msg_id_base) */
    register_interface_handlers();

    return 0;
}

/* ─────────────────────────────────────────────────────────────────────
 * dump_interfaces() — the M / S / PING / S / W pattern
 * ───────────────────────────────────────────────────────────────────── */
static int dump_interfaces(void)
{
    vat_main_t                 *vam = &vat_main;
    vl_api_sw_interface_dump_t *mp;
    vl_api_control_ping_t      *mp_ping;
    int ret;

    /*
     * Point __plugin_msg_base at the interface plugin so that M()
     * computes:  _vl_msg_id = VL_API_SW_INTERFACE_DUMP + interface_msg_id_base
     */
    __plugin_msg_base = interface_msg_id_base;

    /* Step 1: allocate + stamp the dump request */
    M(SW_INTERFACE_DUMP, mp);
    /* sw_if_index is already 0 (from clib_memset); 0 / ~0 both mean "all" */

    /* Step 2: transmit */
    S(mp);

    /*
     * Step 3: send a control_ping immediately after.
     * VPP processes messages in order, so control_ping_reply will only
     * arrive after all sw_interface_details have been sent.
     */
    __plugin_msg_base = memclnt_msg_id_base;
    PING(NULL, mp_ping);
    S(mp_ping);

    /* Step 4: block until control_ping_reply sets result_ready = 1 */
    W(ret);

    return ret;
}

/* ─────────────────────────────────────────────────────────────────────
 * Print results from the hash tables populated by the reply handler
 * ───────────────────────────────────────────────────────────────────── */
static void print_interfaces(void)
{
    vat_main_t  *vam = &vat_main;
    hash_pair_t *p;

    printf("\n%-30s  %s\n", "Interface", "sw_if_index");
    printf("%-30s  %s\n",   "─────────────────────────────", "───────────");

    hash_foreach_pair(p, vam->sw_if_index_by_interface_name, ({
        u8  *name  = (u8 *) (p->key);
        u32  idx   = (u32)   p->value[0];
        printf("%-30s  %u\n", name, idx);
    }));
}

/* ─────────────────────────────────────────────────────────────────────
 * main
 * ───────────────────────────────────────────────────────────────────── */
int main(void)
{
    if (init() != 0)
        return 1;

    printf("Connected to VPP. Dumping interfaces...\n");

    if (dump_interfaces() != 0) {
        fprintf(stderr, "ERROR: sw_interface_dump failed\n");
        return 1;
    }

    print_interfaces();

    return 0;
}

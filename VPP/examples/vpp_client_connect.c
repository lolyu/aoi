#include <assert.h>
#include <stdio.h>
#include <string.h>

// VPP related headers
// vat_main_t
#include <vat/vat.h>
// handlers related
#include <vlibmemory/api.h>
// u8. u32, uword, clib_error_t
#include <vppinfra/error.h>
// M, S, W, WR macros
#include <vlibapi/vat_helper_macros.h>
// vlib api common util
#include <vlibapi/api.h>
// clib_mem_init, vlib_main_init
#include <vlib/vlib.h>

//-----------------------------------------------------------------------------------
/* interface plugin headers */
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
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
/* memory client headers */
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
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
/* VPE base headers */
#include <vnet/format_fns.h>
//-----------------------------------------------------------------------------------

vat_main_t vat_main;
uword *interface_name_by_sw_index = NULL;
u16 interface_msg_id_base;
u16 memclnt_msg_id_base;
u16 __plugin_msg_base; /* used by the M() macro */

void get_msg_id_bases()
{
    u8 *plugin_name;
    // construct a C string ending with '\0'
    plugin_name = format(0, "interface_%08x%c", interface_api_version, 0);
    interface_msg_id_base = vl_client_get_first_plugin_msg_id((char *)plugin_name);
    assert(interface_msg_id_base != (u16)~0);
    vec_free(plugin_name);

    memclnt_msg_id_base = 0;
}

void vl_msg_api_set_handlers(int id, char *name,
                             void *handler, void *cleanup,
                             void *endian, int size,
                             int traced, void *tojson,
                             void *fromjson, void *calc_size, int autoendian)
{
    vl_msg_api_msg_config_t cfg;
    clib_memset(&cfg, 0, sizeof(cfg));
    cfg.id = id;
    cfg.name = name;
    cfg.handler = handler;
    cfg.cleanup = cleanup;
    cfg.endian = endian;
    cfg.traced = traced;
    cfg.replay = 1;
    cfg.tojson = tojson;
    cfg.fromjson = fromjson;
    cfg.calc_size = calc_size;
    cfg.is_autoendian = autoendian;
    vl_msg_api_config(&cfg);
}

void set_reply_status(int retval)
{
    vat_main_t *vam = &vat_main;
    vam->retval      = retval;
    vam->result_ready = 1;
}

// vat_time_now and vat_suspend are used by the W() macro
// #define W(ret)                                                  \
// do {                                                            \
//     f64 timeout = vat_time_now (vam) + 1.0;                     \
//     socket_client_main_t *scm = vam->socket_client_main;	\
//     ret = -99;                                                  \
//                                                                 \
//     if (scm && scm->socket_enable)                              \
//       vl_socket_client_read (5);                       		\
//     while (vat_time_now (vam) < timeout) {                      \
//         if (vam->result_ready == 1) {                           \
//             ret = vam->retval;                                  \
//             break;                                              \
//         }                                                       \
//         vat_suspend (vam->vlib_main, 1e-5);                     \
//     }								\
// } while(0);
// they should be implemented by the application to offer customized polling behavior
// vat_suspend can be implemented in two ways:
// - busy spinning (this demo)
// - yield the CPU

f64 vat_time_now(vat_main_t *vam)
{
    return clib_time_now(&vam->clib_time);
}

void __clib_no_tail_calls vat_suspend(vlib_main_t *vm, f64 interval)
{
    const struct timespec req = { 0, 100000000 }; /* 100 ms */
    nanosleep(&req, NULL);
}

void vl_noop_handler(void *mp) {}

void vl_api_get_first_msg_id_reply_t_handler(vl_api_get_first_msg_id_reply_t *mp)
{
    /* handled internally by vl_client_get_first_plugin_msg_id() */
}

void vl_api_control_ping_reply_t_handler(vl_api_control_ping_reply_t *mp)
{
    vat_main_t *vam = &vat_main;

    set_reply_status(mp->retval);

    if (vam->socket_client_main)
        vam->socket_client_main->control_pings_outstanding--;
}

void vl_api_sw_interface_details_t_handler(vl_api_sw_interface_details_t *mp)
{
    vat_main_t *vam = &vat_main;

    /* format() allocates a null-terminated copy of the interface name */
    u8 *s = format(0, "%s%c", mp->interface_name, 0);

    /* name → index */
    hash_set_mem(vam->sw_if_index_by_interface_name, s, mp->sw_if_index);

    /* index → name */
    hash_set(interface_name_by_sw_index, mp->sw_if_index, s);
}

void register_base_handlers()
{
    // the get first message id request is not used here
    // the base message ids are requested via get_msg_id_bases()
    vl_msg_api_set_handlers(VL_API_GET_FIRST_MSG_ID_REPLY + 1,
                            "get_first_msg_id_reply",
                            vl_api_get_first_msg_id_reply_t_handler,
                            vl_noop_handler,
                            vl_api_get_first_msg_id_reply_t_endian,
                            sizeof(vl_api_get_first_msg_id_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_get_first_msg_id_reply_t_calc_size,
                            0);

    vl_msg_api_set_handlers(VL_API_CONTROL_PING_REPLY + 1,
                            "control_ping_reply",
                            vl_api_control_ping_reply_t_handler,
                            vl_noop_handler,
                            vl_api_control_ping_reply_t_endian,
                            sizeof(vl_api_control_ping_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_control_ping_reply_t_calc_size,
                            1);
}

void register_interface_handlers()
{
    vl_msg_api_set_handlers((VL_API_SW_INTERFACE_DETAILS + interface_msg_id_base),
                            "sw_interface_details",
                            vl_api_sw_interface_details_t_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_details_t_endian,
                            sizeof(vl_api_sw_interface_details_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_details_t_calc_size,
                            1);
}

int connect_to_vpp(const char *client_name)
{
    // vat is the VPP client context (vat_main_t)
    vat_main_t *vam = &vat_main;
    // am is the VPP engine context (api_main_t)
    api_main_t *am = vlibapi_get_main();

    // bootstrap the VPP heap
    clib_mem_init(0, 128 << 20);
    // init the vlib global main
    vlib_main_init();
    // init the time counter
    clib_time_init(&vam->clib_time);
    // set the plugin message start id
    vl_msg_api_set_first_available_msg_id(VL_MSG_MEMCLNT_LAST + 1);

    // static register for memory client handlers
    register_base_handlers();

    // socket_client_main is the global low-level socket state that stores:
    // - file descriptor
    // - tx/rx bufers
    // - client_index assigned by VPP during the handshake
    // assign socket_client_main to vam to expose low level socket status
    vam->socket_client_main = &socket_client_main;
    vam->socket_name = (u8 *)"/run/vpp/api.sock";

    // init hash tables
    vam->sw_if_index_by_interface_name = hash_create_string(0, sizeof(uword));
    interface_name_by_sw_index = hash_create(0, sizeof(uword));

    if (vl_socket_client_connect((char *)vam->socket_name, (char *)client_name, 0))
    {
        return -1;
    }

    // after connect, the client index is stored in socket_client_main.client_index
    // stores the client index into the client/engine context for late communications
    vam->my_client_index = htonl(socket_client_main.client_index);
    am->my_client_index = htonl(socket_client_main.client_index);

    // init message id bases
    // - interface plugin
    // - memory client
    get_msg_id_bases();

    // dynamic register for plugin handlers after the message id bases are resolved
    register_interface_handlers();

    return 0;
}

int dump_interfaces(void)
{
    vat_main_t *vam = &vat_main;
    vl_api_sw_interface_dump_t *mp;
    vl_api_control_ping_t *mp_ping;
    int ret;

    __plugin_msg_base = interface_msg_id_base;
    M(SW_INTERFACE_DUMP, mp);
    S(mp);

    __plugin_msg_base = memclnt_msg_id_base;
    PING(NULL, mp_ping);
    S(mp_ping);

    W(ret)

    return ret;
}

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

int main(int argc, char *argv[])
{
    connect_to_vpp("demo");

    if (dump_interfaces() != 0) {
        fprintf(stderr, "ERROR: sw_interface_dump failed\n");
        return 1;
    }

    print_interfaces();

    return 0;
}
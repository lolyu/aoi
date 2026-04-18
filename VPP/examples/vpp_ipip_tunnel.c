#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

// VPP related headers
#include <vat/vat.h>
#include <vlibmemory/api.h>
#include <vppinfra/error.h>
#include <vlibapi/vat_helper_macros.h>
#include <vlibapi/api.h>
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
/* ipip plugin headers */
#include <vnet/ipip/ipip.api_enum.h>
#include <vnet/ipip/ipip.api_types.h>

#define vl_typedefs
#include <vnet/ipip/ipip.api.h>
#undef vl_typedefs

#define vl_endianfun
#include <vnet/ipip/ipip.api.h>
#undef vl_endianfun

#define vl_calcsizefun
#include <vnet/ipip/ipip.api.h>
#undef vl_calcsizefun

#define vl_api_version(n, v) static u32 ipip_api_version = v;
#include <vnet/ipip/ipip.api.h>
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
u16 ipip_msg_id_base;
u16 memclnt_msg_id_base;
u16 __plugin_msg_base; /* used by the M() macro */

/* sw_if_index of the newly created ipip tunnel, filled in by the reply handler */
static u32 ipip_tunnel_sw_if_index = ~0;

void get_msg_id_bases()
{
    u8 *plugin_name;

    plugin_name = format(0, "interface_%08x%c", interface_api_version, 0);
    interface_msg_id_base = vl_client_get_first_plugin_msg_id((char *)plugin_name);
    assert(interface_msg_id_base != (u16)~0);
    vec_free(plugin_name);

    plugin_name = format(0, "ipip_%08x%c", ipip_api_version, 0);
    ipip_msg_id_base = vl_client_get_first_plugin_msg_id((char *)plugin_name);
    assert(ipip_msg_id_base != (u16)~0);
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
    cfg.id           = id;
    cfg.name         = name;
    cfg.handler      = handler;
    cfg.cleanup      = cleanup;
    cfg.endian       = endian;
    cfg.traced       = traced;
    cfg.replay       = 1;
    cfg.tojson       = tojson;
    cfg.fromjson     = fromjson;
    cfg.calc_size    = calc_size;
    cfg.is_autoendian = autoendian;
    vl_msg_api_config(&cfg);
}

void set_reply_status(int retval)
{
    vat_main_t *vam     = &vat_main;
    vam->retval         = retval;
    vam->result_ready   = 1;
}

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
    u8 *s = format(0, "%s%c", mp->interface_name, 0);
    hash_set_mem(vam->sw_if_index_by_interface_name, s, mp->sw_if_index);
    hash_set(interface_name_by_sw_index, mp->sw_if_index, s);
}

/*
 * ipip_add_tunnel reply: stores the sw_if_index of the new tunnel
 */
void vl_api_ipip_add_tunnel_reply_t_handler(vl_api_ipip_add_tunnel_reply_t *mp)
{
    /* autoendian=1: message already in host byte order, do NOT ntohl */
    if (mp->retval == 0)
        ipip_tunnel_sw_if_index = mp->sw_if_index;
    set_reply_status(mp->retval);
}

/*
 * Simple autoreply handler used for sw_interface_set_flags,
 * sw_interface_set_table, and sw_interface_set_unnumbered.
 */
void vl_api_sw_interface_set_flags_reply_t_handler(vl_api_sw_interface_set_flags_reply_t *mp)
{
    set_reply_status(mp->retval);
}

void vl_api_sw_interface_set_table_reply_t_handler(vl_api_sw_interface_set_table_reply_t *mp)
{
    set_reply_status(mp->retval);
}

void vl_api_sw_interface_set_unnumbered_reply_t_handler(vl_api_sw_interface_set_unnumbered_reply_t *mp)
{
    set_reply_status(mp->retval);
}

void register_base_handlers()
{
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

    vl_msg_api_set_handlers((VL_API_SW_INTERFACE_SET_FLAGS_REPLY + interface_msg_id_base),
                            "sw_interface_set_flags_reply",
                            vl_api_sw_interface_set_flags_reply_t_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_flags_reply_t_endian,
                            sizeof(vl_api_sw_interface_set_flags_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_flags_reply_t_calc_size,
                            1);

    vl_msg_api_set_handlers((VL_API_SW_INTERFACE_SET_TABLE_REPLY + interface_msg_id_base),
                            "sw_interface_set_table_reply",
                            vl_api_sw_interface_set_table_reply_t_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_table_reply_t_endian,
                            sizeof(vl_api_sw_interface_set_table_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_table_reply_t_calc_size,
                            1);

    vl_msg_api_set_handlers((VL_API_SW_INTERFACE_SET_UNNUMBERED_REPLY + interface_msg_id_base),
                            "sw_interface_set_unnumbered_reply",
                            vl_api_sw_interface_set_unnumbered_reply_t_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_unnumbered_reply_t_endian,
                            sizeof(vl_api_sw_interface_set_unnumbered_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_sw_interface_set_unnumbered_reply_t_calc_size,
                            1);
}

void register_ipip_handlers()
{
    vl_msg_api_set_handlers((VL_API_IPIP_ADD_TUNNEL_REPLY + ipip_msg_id_base),
                            "ipip_add_tunnel_reply",
                            vl_api_ipip_add_tunnel_reply_t_handler,
                            vl_noop_handler,
                            vl_api_ipip_add_tunnel_reply_t_endian,
                            sizeof(vl_api_ipip_add_tunnel_reply_t),
                            1,
                            vl_noop_handler,
                            vl_noop_handler,
                            vl_api_ipip_add_tunnel_reply_t_calc_size,
                            1);
}

int connect_to_vpp(const char *client_name)
{
    vat_main_t *vam = &vat_main;
    api_main_t *am  = vlibapi_get_main();

    clib_mem_init(0, 128 << 20);
    vlib_main_init();
    clib_time_init(&vam->clib_time);
    vl_msg_api_set_first_available_msg_id(VL_MSG_MEMCLNT_LAST + 1);

    register_base_handlers();

    vam->socket_client_main = &socket_client_main;
    vam->socket_name        = (u8 *)"/run/vpp/api.sock";

    vam->sw_if_index_by_interface_name = hash_create_string(0, sizeof(uword));
    interface_name_by_sw_index         = hash_create(0, sizeof(uword));

    if (vl_socket_client_connect((char *)vam->socket_name, (char *)client_name, 0))
        return -1;

    vam->my_client_index = htonl(socket_client_main.client_index);
    am->my_client_index  = htonl(socket_client_main.client_index);

    get_msg_id_bases();

    register_interface_handlers();
    register_ipip_handlers();

    return 0;
}

/*
 * Populate vam->sw_if_index_by_interface_name by calling sw_interface_dump.
 * Used to resolve bvi1000 → sw_if_index before issuing set_unnumbered.
 */
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

    W(ret);
    return ret;
}

/*
 * create ipip tunnel src 192.168.0.1 p2mp outer-table-id 0
 * Returns sw_if_index of the new tunnel on success, ~0 on failure.
 */
int create_ipip_p2mp_tunnel(const char *src_addr, u32 outer_table_id)
{
    vat_main_t *vam = &vat_main;
    vl_api_ipip_add_tunnel_t *mp;
    int ret;

    __plugin_msg_base = ipip_msg_id_base;
    M(IPIP_ADD_TUNNEL, mp);

    mp->tunnel.instance = htonl(~0);               /* auto-assign */
    mp->tunnel.mode     = TUNNEL_API_MODE_MP;       /* P2MP */
    mp->tunnel.table_id = htonl(outer_table_id);
    mp->tunnel.flags    = 0;
    mp->tunnel.dscp     = 0;

    /* src: IPv4 address encoded in vl_api_address_t */
    mp->tunnel.src.af = ADDRESS_IP4;
    inet_pton(AF_INET, src_addr, mp->tunnel.src.un.ip4);

    /* dst must be 0.0.0.0 for P2MP */
    mp->tunnel.dst.af = ADDRESS_IP4;
    clib_memset(mp->tunnel.dst.un.ip4, 0, sizeof(mp->tunnel.dst.un.ip4));

    S(mp);
    W(ret);
    return ret;
}

/*
 * set interface state <if> up
 */
int set_interface_up(u32 sw_if_index)
{
    vat_main_t *vam = &vat_main;
    vl_api_sw_interface_set_flags_t *mp;
    int ret;

    __plugin_msg_base = interface_msg_id_base;
    M(SW_INTERFACE_SET_FLAGS, mp);
    mp->sw_if_index = htonl(sw_if_index);
    mp->flags       = htonl(IF_STATUS_API_FLAG_ADMIN_UP);
    S(mp);
    W(ret);
    return ret;
}

/*
 * set interface ip table <if> <vrf_id>
 */
int set_interface_ip_table(u32 sw_if_index, u32 vrf_id)
{
    vat_main_t *vam = &vat_main;
    vl_api_sw_interface_set_table_t *mp;
    int ret;

    __plugin_msg_base = interface_msg_id_base;
    M(SW_INTERFACE_SET_TABLE, mp);
    mp->sw_if_index = htonl(sw_if_index);
    mp->is_ipv6     = 0;
    mp->vrf_id      = htonl(vrf_id);
    S(mp);
    W(ret);
    return ret;
}

/*
 * set interface unnumbered <borrower_if> use <donor_if>
 *
 * sw_if_index        = donor  (the interface whose address is borrowed, e.g. bvi1000)
 * unnumbered_sw_if_index = borrower (the unnumbered interface, e.g. ipip0)
 */
int set_interface_unnumbered(u32 borrower_sw_if_index, u32 donor_sw_if_index)
{
    vat_main_t *vam = &vat_main;
    vl_api_sw_interface_set_unnumbered_t *mp;
    int ret;

    __plugin_msg_base = interface_msg_id_base;
    M(SW_INTERFACE_SET_UNNUMBERED, mp);
    mp->sw_if_index           = htonl(donor_sw_if_index);
    mp->unnumbered_sw_if_index = htonl(borrower_sw_if_index);
    mp->is_add                = 1;
    S(mp);
    W(ret);
    return ret;
}

int main(int argc, char *argv[])
{
    vat_main_t *vam = &vat_main;
    uword *p;
    u32 bvi1000_sw_if_index;

    if (connect_to_vpp("ipip-tunnel-demo") != 0)
    {
        fprintf(stderr, "ERROR: failed to connect to VPP\n");
        return 1;
    }

    /* Populate the interface name→index map so we can look up bvi1000. */
    if (dump_interfaces() != 0)
    {
        fprintf(stderr, "ERROR: sw_interface_dump failed\n");
        return 1;
    }

    /* Resolve bvi1000 */
    p = hash_get_mem(vam->sw_if_index_by_interface_name, "bvi1000");
    if (!p)
    {
        fprintf(stderr, "ERROR: interface bvi1000 not found\n");
        return 1;
    }
    bvi1000_sw_if_index = (u32)p[0];
    printf("bvi1000 sw_if_index = %u\n", bvi1000_sw_if_index);

    /* 1. create ipip tunnel src 192.168.0.1 p2mp outer-table-id 0 */
    if (create_ipip_p2mp_tunnel("192.168.0.1", 0) != 0)
    {
        fprintf(stderr, "ERROR: ipip_add_tunnel failed\n");
        return 1;
    }
    printf("Created P2MP ipip tunnel: sw_if_index = %u\n", ipip_tunnel_sw_if_index);

    /* 2. set interface state ipip0 up */
    if (set_interface_up(ipip_tunnel_sw_if_index) != 0)
    {
        fprintf(stderr, "ERROR: sw_interface_set_flags failed\n");
        return 1;
    }
    printf("Interface admin-up\n");

    /* 3. set interface ip table ipip0 0 */
    if (set_interface_ip_table(ipip_tunnel_sw_if_index, 0) != 0)
    {
        fprintf(stderr, "ERROR: sw_interface_set_table failed\n");
        return 1;
    }
    printf("Interface assigned to VRF 0\n");

    /* 4. set interface unnumbered ipip0 use bvi1000 */
    if (set_interface_unnumbered(ipip_tunnel_sw_if_index, bvi1000_sw_if_index) != 0)
    {
        fprintf(stderr, "ERROR: sw_interface_set_unnumbered failed\n");
        return 1;
    }
    printf("ipip tunnel set unnumbered using bvi1000\n");

    return 0;
}

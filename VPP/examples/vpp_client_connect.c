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

vat_main_t vat_main;

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


    // socket_client_main is the global low-level socket state that stores:
    // - file descriptor
    // - tx/rx bufers
    // - client_index assigned by VPP during the handshake
    // assign socket_client_main to vam to expose low level socket status
    vam->socket_client_main = &socket_client_main;
    vam->socket_name = (u8 *) "/run/vpp/api.sock";

    if (vl_socket_client_connect((char *) vam->socket_name, (char *) client_name, 0))
    {
        return -1;
    }

    // after connect, the client index is stored in socket_client_main.client_index
    // stores the client index into the client/engine context for late communications

    vam->my_client_index = htonl(socket_client_main.client_index);
    am->my_client_index = htonl(socket_client_main.client_index);

    return 0;
}

int main(int argc, char *argv[])
{
    connect_to_vpp("demo");
    return 0;
}
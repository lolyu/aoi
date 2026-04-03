# VPP Headers Cheatsheet

## Namespace Overview

| Namespace | Layer | Key Headers | Provides |
|---|---|---|---|
| `vppinfra/` | Foundation | `error.h`, `mem.h`, `vec.h`, `types.h` | Primitive types (`u8`/`u32`/`uword`), VPP heap (`clib_mem_init`), vec/pool/hash data structures, `clib_time_t` |
| `vlib/` | Runtime | `vlib.h` | VPP node graph runtime, `vlib_main_init()`, buffer management — needed to bootstrap the global `vlib_global_main` before any API traffic |
| `vlibapi/` | API dispatch | `api.h`, `vat_helper_macros.h` | `api_main_t` (transport/dispatch engine), `vl_msg_api_set_handlers()`, `M()`/`S()`/`W()`/`PING()` send/receive macros, `vlibapi_get_main()` |
| `vlibmemory/` | Transport | `api.h`, `memclnt.api_enum.h` | Socket/SHM client connect, `vl_socket_client_connect()`, `socket_client_main`, `VL_MSG_MEMCLNT_LAST`, handler registration for control_ping |
| `vat/` | Application | `vat.h` | `vat_main_t` — the per-application session object that glues everything: holds `socket_client_main*`, `my_client_index`, `result_ready`/`retval` for `W()`, `clib_time`, interface hash tables. Must be defined by each binary (`vat_main_t vat_main;`) |

## Dependency Order (bottom → top)

```
vppinfra  ←  foundation types & heap
   vlib   ←  runtime init (depends on vppinfra)
 vlibapi  ←  dispatch engine (depends on vlib)
vlibmemory←  socket/SHM transport (depends on vlibapi)
    vat   ←  application session struct (depends on all above)
```

## Minimum Headers to Connect via Socket

```c
#include <vat/vat.h>                          // vat_main_t, vat_main (extern)
#include <vlibmemory/api.h>                   // vl_socket_client_connect, socket_client_main
#include <vlibapi/api.h>                      // vlibapi_get_main, api_main_t
#include <vlibapi/vat_helper_macros.h>        // M/S/W macros
#include <vlib/vlib.h>                        // clib_mem_init, vlib_main_init
#include <vlibmemory/memclnt.api_enum.h>      // VL_MSG_MEMCLNT_LAST
```

## Required Init Sequence (before any API call)

```c
vat_main_t vat_main;          // define storage — not in any library

clib_mem_init(0, 128 << 20);  // bootstrap VPP heap (must be first)
vlib_main_init();             // init vlib_global_main
clib_time_init(&vam->clib_time);
vl_msg_api_set_first_available_msg_id(VL_MSG_MEMCLNT_LAST + 1);
```

## Gotchas

- `vat_main` is declared `extern` in `<vat/vat.h>` — each binary must define it once: `vat_main_t vat_main;`
- `clib_mem_init()` must run before **any** vec/pool allocation (including inside `vl_socket_client_connect`)
- `memclnt.api.h` doesn't generate `_tojson`/`_fromjson` — use `vl_noop_handler` for those slots in `vl_msg_api_set_handlers()`
- VPP headers use `template`/`class`/`new` as identifiers → **C only**, never include them from `.cpp`
- `clib_mem_init_thread_safe` was removed in VPP 26.02; use `clib_mem_init` or `clib_mem_init_ex`

## C++ Interop Pattern (syncd style)

VPP headers cannot be included from C++ files. The pattern used by syncd-vpp:

1. **`.c` shim files** (`SaiVppXlate.c`) — include all VPP headers, implement all VPP calls
2. **Interface header** (`SaiVppXlate.h`) — exposes only plain C types + `extern "C" { ... }` wrapper, zero VPP internals
3. **`.cpp` files** — include only the interface header; call shim functions via C linkage

# VPP API Message: Format, Send, and Receive

## Client Connect
* two client types:
  * socket client
  * memory client

VPP provides two binary API transport mechanisms. They expose the same `M`/`S`/`W`/`PING` macro interface but differ significantly in how messages are exchanged underneath.

---

### Memory Client (Shared Memory)

The memory client, implemented in memory_client.c, communicates via a **POSIX shared memory region** (SVM — Shared Virtual Memory) that VPP maps into both its own address space and the client's address space. The client registers itself in this region by calling `vl_client_connect()`, which creates a dedicated input queue (`svm_queue_t`) for this client inside the shared region. Sending a message means writing a pointer into VPP's input queue; receiving means VPP writes a message into the client's queue. Because both sides share the same memory, there is **zero copy** — messages are passed as pointers, not bytes.

To drain the receive queue, a **background pthread** (`rx_thread_fn`) is spawned automatically by `vl_client_connect_to_vlib()`. This thread runs a tight loop, reading from the client's SHM queue and dispatching each message through the handler table. Your foreground thread's `W()` then just spins on `result_ready`, which the background thread sets after calling the handler. This model is inherently multi-threaded and requires careful synchronization if multiple foreground threads make API calls.

The SHM region is identified by an SVM name (e.g. `"pvt"`) and accessed via a filesystem path under shm or a VPP-configured directory. This restricts the memory client to processes running on the **same host as VPP** with the appropriate permissions.

---

### Socket Client (Unix Domain Socket)

The socket client, implemented in socket_client.c, communicates via a **Unix domain stream socket** at api.sock. Messages are serialized into a length-prefixed byte stream, written with `vl_socket_client_write()`, and received with `vl_socket_client_read()`. Unlike the SHM client, every message is fully **copied** across the socket boundary.

There is **no background thread**. Instead, `vl_socket_client_read()` is called explicitly inside the `W()` and `WR()` macros — it performs a blocking `read()` with a timeout, then dispatches whatever messages have arrived before returning. This makes the socket client naturally **single-threaded and synchronous**: the calling thread sends, then blocks in `W()` until the reply arrives and the handler sets `result_ready`. This is the design SaiVppXlate.c embraces, protecting all API calls with a mutex to serialize access to the single `vat_main`.

The connection begins with a `SOCKCLNT_CREATE` handshake, during which VPP assigns the client its `client_index`. This index is then used to route replies back to the correct client. Because it is just a file descriptor, the socket can be bind-mounted into a container or forwarded over SSH, offering more deployment flexibility than SHM.

---

### Side-by-Side Comparison

| | Memory Client | Socket Client |
|---|---|---|
| Transport | POSIX SHM (`svm_queue_t`) | Unix domain socket |
| Message passing | Zero-copy (pointer in shared region) | Copy (byte stream) |
| Receive model | Background `pthread` drains queue continuously | Explicit `vl_socket_client_read()` inside `W()` |
| Thread model | Multi-threaded (rx thread + caller thread) | Single-threaded / synchronous |
| Connect function | `vl_client_connect_to_vlib()` | `vl_socket_client_connect()` |
| Send function | `vl_msg_api_send_shmem()` | `vl_socket_client_write()` |
| Alloc function | `vl_msg_api_alloc_as_if_client()` | `vl_socket_client_msg_alloc()` |
| Requires same host | Yes | Yes (or forwarded fd) |
| Container-friendly | No (SHM namespace must be shared) | Yes |
| Performance | Higher (zero-copy, low latency) | Lower (copy + syscall per message) |
| Complexity | Higher (threading, SHM mapping) | Lower (single-threaded, file descriptor) |
| Used by | `vat` CLI tool | SaiVppXlate.c, list_interfaces.c |

The `M`/`S`/`W`/`PING` macros transparently select the right path by checking `scm->socket_enable` — so application code is identical regardless of which transport is active underneath.

## Message Format

Every VPP API message — request or reply — is a **packed C struct** auto-generated from `.api` definition files. All structs share a standard two-field header:

```c
/* Common header — present in every message */
u16  _vl_msg_id;      /* Message type ID, in network byte order */
u32  client_index;    /* Identifies the connected client */
```

VPP's message ID space is partitioned: the memclnt range [0, `VL_MSG_MEMCLNT_LAST`] is statically reserved for core control messages (control_ping, get_first_msg_id, memclnt_create, etc.). Everything above that is available for plugin/application-defined messages.

Client needs to tell the VPP API dispatch engine to start allocating dynamic message IDs for plugin messages after the reserved range:

```c
vl_msg_api_set_first_available_msg_id(VL_MSG_MEMCLNT_LAST + 1);
```

Without this call, the dispatcher's "next free ID" counter starts at 0, and any subsequent `vl_msg_api_get_msg_ids()` call for a plugin file could hand out IDs that collide with the memclnt range — causing the wrong handler to fire when a reply arrives. By passing `VL_MSG_MEMCLNT_LAST` + 1, you fence off the reserved block and guarantee all dynamically registered messages get non-colliding IDs.

Request messages additionally carry a `context` field that VPP echoes back in the reply, allowing the client to correlate replies to requests. The rest of the struct is message-specific payload. For example:

```c
/* Request: dump all software interfaces */
typedef struct {
    u16  _vl_msg_id;
    u32  client_index;
    u32  context;
    vl_api_interface_index_t sw_if_index;  /* 0xFFFFFFFF = all */
} vl_api_sw_interface_dump_t;

/* Reply: one instance per interface */
typedef struct {
    u16  _vl_msg_id;
    u32  client_index;
    u32  context;
    vl_api_interface_index_t sw_if_index;
    u8   interface_name[64];
    /* ... more fields ... */
} vl_api_sw_if_details_t;
```

All multi-byte integer fields are in **network byte order** (big-endian). The endian-swap function registered per message type (`vl_api_##n##_t_endian`) handles the conversion automatically on the receive path.

The `_vl_msg_id` value is **not a simple enum**. For plugin messages it is computed as `VL_API_<MSG_NAME> + <plugin_msg_id_base>`, where the base is a dynamic offset assigned by VPP at runtime when the plugin is loaded. Base VPE messages (like `CONTROL_PING`) use fixed IDs from the memclnt range.

### Message ID Allocation Deep Dive

#### memclnt Messages — The Built-in Core

All messages defined in `memclnt.api` receive **static, compile-time IDs** via the `vl_api_memclnt_enum_t` enum. These occupy range `[0, VL_MSG_MEMCLNT_LAST)` and are the same on every VPP instance regardless of which plugins are loaded.

**Full message list (enum value → wire ID is `enum + 1`):**

| Enum (0-based) | Message | Direction | Purpose |
|---|---|---|---|
| 0 | `memclnt_create` | C→V | Register a shared-memory client (deprecated, use v2) |
| 1 | `memclnt_create_reply` | V→C | Returns client handle, index, serialized message table |
| 2 | `memclnt_delete` | C→V | Unregister a shared-memory client |
| 3 | `memclnt_delete_reply` | V→C | Confirms deletion |
| 4 | `rx_thread_exit` | internal | Tells the client's rx thread to exit |
| 5 | `memclnt_rx_thread_suspend` | internal | Suspends the client's rx thread |
| 6 | `memclnt_read_timeout` | internal | Signals a read timeout |
| 7 | `rpc_call` | C→V | Execute an arbitrary function pointer on VPP's main thread |
| 8 | `rpc_call_reply` | V→C | RPC result |
| 9 | `get_first_msg_id` | C→V | Look up a plugin's dynamic message-ID base by versioned name |
| 10 | `get_first_msg_id_reply` | V→C | Returns `first_msg_id` for the plugin, or error if CRC mismatch |
| 11 | `api_versions` | C→V | Request version table for all loaded API modules |
| 12 | `api_versions_reply` | V→C | Returns array of `(major, minor, patch, name)` per module |
| 13 | `trace_plugin_msg_ids` | C→V | Record plugin name↔ID range mapping (for API trace replay) |
| 14 | `sockclnt_create` | C→V | Register a socket client (the socket handshake message) |
| 15 | `sockclnt_create_reply` | V→C | Returns `client_index`, full message table |
| 16 | `sockclnt_delete` | C→V | Unregister a socket client |
| 17 | `sockclnt_delete_reply` | V→C | Confirms deletion |
| 18 | `sock_init_shm` | C→V | Upgrade a socket connection to shared-memory transport |
| 19 | `sock_init_shm_reply` | V→C | Result |
| 20 | `memclnt_keepalive` | V→C | Ping from VPP to detect dead clients on idle connections |
| 21 | `memclnt_keepalive_reply` | C→V | Client's pong |
| 22 | `control_ping` | C→V | Client-initiated ping; used as a dump fence |
| 23 | `control_ping_reply` | V→C | Confirms all preceding replies have been sent |
| 24 | `memclnt_create_v2` | C→V | Register a shared-memory client (current version, adds keepalive flag) |
| 25 | `memclnt_create_v2_reply` | V→C | Same as v1 reply |
| 26 | `get_api_json` | C→V | Request JSON schema of loaded APIs |
| 27 | `get_api_json_reply` | V→C | Returns JSON string |
| — | `VL_MSG_MEMCLNT_LAST` | — | Sentinel; first available ID for plugins |

**Key messages explained:**

**`sockclnt_create` / `sockclnt_create_reply`** — The socket handshake. When `vl_socket_client_connect()` opens the Unix socket, it sends `sockclnt_create` with the client name. VPP replies with `sockclnt_create_reply` containing the assigned `client_index` (used in every subsequent message) and the **full message table** — an array of `(index, name)` pairs listing every registered message ID and its name. This is the same table shown by `vppctl show api message`.

**`get_first_msg_id` / `get_first_msg_id_reply`** — Plugin ID resolution. The client sends a versioned plugin name (e.g., `"interface_f29d6f21"`). VPP looks up the plugin's registered message block and returns its `first_msg_id` (the base offset for that plugin's messages). If the CRC doesn't match any loaded plugin, `retval` is negative and `first_msg_id` is `(u16)~0`. Note: `vl_client_get_first_plugin_msg_id()` temporarily swaps in its own internal handler for this reply — the handler your code registers is a placeholder that is never actually called.

**`control_ping` / `control_ping_reply`** — The dump fence. Since dump APIs produce N replies with no end-of-stream marker, the client sends `control_ping` after the dump request. VPP processes messages in order, so `control_ping_reply` only arrives after all dump replies. The control_ping_reply handler does two critical things:
1. Sets `vam->result_ready = 1` → unblocks the `W()` poll loop
2. Decrements `control_pings_outstanding` → tells `vl_socket_client_read_internal()` to stop blocking

**`memclnt_keepalive` / `memclnt_keepalive_reply`** — Dead client detection. VPP periodically sends `memclnt_keepalive` to idle clients. If the client doesn't reply, VPP marks it dead and cleans up its registration. This is a **server→client** message (unlike `control_ping` which is client→server).

**`memclnt_create` vs `sockclnt_create`** — The SHM client uses `memclnt_create` (or `memclnt_create_v2`); the socket client uses `sockclnt_create`. Both achieve the same result (register a client, get a `client_index`) but the handshake details differ — SHM passes a queue pointer, socket passes a name. Your code never sends these manually; they are handled internally by `vl_client_connect_to_vlib()` and `vl_socket_client_connect()`.

**Wire ID = enum + 1:** memclnt message IDs on the wire are **1-based** (the `+1` in `vl_msg_api_set_handlers(VL_API_CONTROL_PING_REPLY + 1, ...)`). This is because VPP reserves wire ID 0 as "invalid/unset". The `PING()` macro hardcodes this: `mp_ping->_vl_msg_id = htons(VL_API_CONTROL_PING + 1)`.

---

VPP assigns message IDs in two tiers:

**Tier 1 — Static (memclnt range):** IDs `[0, VL_MSG_MEMCLNT_LAST]` are hardcoded for core messages (`control_ping`, `get_first_msg_id`, `memclnt_create`, etc.). These never change across restarts or versions. `memclnt_msg_id_base` is always 0.

**Tier 2 — Dynamic (plugin ranges):** When VPP loads a plugin, the plugin calls `vl_msg_api_get_msg_ids("<name>_<crc>", count)` on the server side to claim a contiguous block of `count` IDs. The allocation is first-come-first-served, so the base depends on plugin load order. You can inspect the current allocation with:

```
vpp# show api plugin
Name                                               First-ID  Last-ID
acl_9cde599d                                             56       97
af_packet_720ee900                                      882      893
interface_f29d6f21                                      200      250
```

Each plugin's block spans `[First-ID, Last-ID]` where `Last-ID = First-ID + message_count - 1`. The blocks are non-contiguous and unordered — `acl` starts at 56, `interface` at 200, `af_packet` at 882.

**How `_vl_msg_id` maps to a plugin range:**

When the client sends a message, `M()` computes `_vl_msg_id = VL_API_<MSG> + __plugin_msg_base`. For example, if `VL_API_SW_INTERFACE_DUMP` = 2 and `interface_msg_id_base` (First-ID) = 200, then `_vl_msg_id` = 202.

When VPP receives it, it finds 202 falls in the interface plugin's `[200, 250]` range, subtracts the base to get local offset 2, and dispatches to the handler for `SW_INTERFACE_DUMP`. The same applies to replies — the reply's `_vl_msg_id` also uses the plugin's range, which is why `register_interface_handlers` registers handlers at `INTERFACE_MSG_ID(SW_INTERFACE_DETAILS)` = `VL_API_SW_INTERFACE_DETAILS + interface_msg_id_base`.

**Client-side resolution — the versioned lookup:**

The client resolves a plugin's First-ID at connect time via:

```c
name = format(0, "interface_%08x%c", interface_api_version, 0);
interface_msg_id_base = vl_client_get_first_plugin_msg_id((char *) name);
```

The `%08x` is a CRC computed over the `.api` file's message definitions (names, field types, field order). This serves dual purpose:
1. **Lookup key** — finds where VPP placed the plugin's message block this run
2. **Compatibility gate** — if the client was compiled against a different `.api` version than the server, the CRC won't match, `vl_client_get_first_plugin_msg_id` returns `(u16)~0`, and the `assert()` crashes the client immediately rather than allowing silent data corruption from mismatched struct layouts

The versioned string (e.g., `"interface_f29d6f21"`) is built from:
- The `.api` file basename (`interface`)
- The CRC captured at compile time via:
  ```c
  #define vl_api_version(n, v) static u32 interface_api_version = v;
  #include <vnet/interface.api.h>
  #undef vl_api_version
  ```

---

## Sending a Request

The send path is captured in four macros from `vat_helper_macros.h`:

**Step 1 — Allocate and populate (`M`):**

```c
M(SW_INTERFACE_DUMP, mp);
```

This macro allocates a zero-filled message buffer (either from the socket write buffer or the shared-memory allocator), stamps `_vl_msg_id` with the correct value in network byte order, and sets `client_index`.

```c
#define M(T, mp)                                                \
do {                                                            \
    vam->result_ready = 0;                                      \
    if (scm && scm->socket_enable)                              \
      mp = vl_socket_client_msg_alloc(sizeof(*mp));             \
    else                                                        \
      mp = vl_msg_api_alloc_as_if_client(sizeof(*mp));          \
    clib_memset(mp, 0, sizeof(*mp));                            \
    mp->_vl_msg_id = ntohs(VL_API_##T + __plugin_msg_base);    \
    mp->client_index = vam->my_client_index;                    \
} while(0);
```

**Step 2 — Fill payload fields manually:**

```c
mp->sw_if_index = htonl(~0);  /* ~0 = dump all interfaces */
```

**Step 3 — Transmit (`S`):**

```c
S(mp);
```

Over a socket connection this calls `vl_socket_client_write()`, which flushes the buffer to VPP's API Unix domain socket (`/run/vpp/api.sock`). Over shared memory it calls `vl_msg_api_send_shmem()`.

```c
#define S(mp)                                                   \
do {                                                            \
  if (scm && scm->socket_enable)                                \
    vl_socket_client_write();                                   \
  else                                                          \
    vl_msg_api_send_shmem(vam->vl_input_queue, (u8 *)&mp);      \
} while(0);
```

---

## Receiving Replies

VPP replies asynchronously. The client dispatches received messages by calling `vl_socket_client_read()`, which reads from the socket and invokes the registered handler for each message's `_vl_msg_id`. Handlers are registered during init via `vl_msg_api_set_handlers()`.

### Handler Registration (`vl_msg_api_set_handlers`)

Before any reply can be dispatched, each message type must be registered with the API dispatch engine. This is done via a wrapper around `vl_msg_api_config()`:

```c
void vl_msg_api_set_handlers(int id, const char *name,
                             void *handler, void *cleanup,
                             void *endian, int size,
                             int traced, void *tojson,
                             void *fromjson, void *calc_size)
{
    vl_msg_api_msg_config_t cfg;
    clib_memset(&cfg, 0, sizeof(cfg));
    cfg.id        = id;
    cfg.name      = name;
    cfg.handler   = handler;
    cfg.cleanup   = cleanup;
    cfg.endian    = endian;
    cfg.traced    = traced;
    cfg.replay    = 1;
    cfg.tojson    = tojson;
    cfg.fromjson  = fromjson;
    cfg.calc_size = calc_size;
    cfg.is_autoendian = 1;
    vl_msg_api_config(&cfg);
}
```

Each field in `vl_msg_api_msg_config_t`:

| Field | Purpose |
|---|---|
| `id` | Runtime message ID — the same value that appears in `_vl_msg_id` on the wire. For memclnt messages: `VL_API_<MSG> + 1` (1-based indexing). For plugin messages: `VL_API_<MSG> + plugin_msg_id_base` |
| `name` | Debug/trace string, e.g. `"control_ping_reply"` |
| `handler` | Callback invoked when a message with this ID arrives, e.g. `vl_api_control_ping_reply_t_handler` |
| `cleanup` | Called to free resources after the handler runs. Typically `vl_noop_handler` for client-side code |
| `endian` | Byte-swap function (`vl_api_<msg>_t_endian`) that converts all multi-byte fields from network to host order before the handler sees them |
| `size` | `sizeof(vl_api_<msg>_t)` — used for validation and trace buffer sizing |
| `traced` | Whether API tracing records this message (1 = yes) |
| `tojson` | JSON serializer. Memclnt messages don't generate these (headers not included with `vl_jsonfun` gate), so `vl_noop_handler` is used as a stub |
| `fromjson` | JSON deserializer. Same story — `vl_noop_handler` |
| `calc_size` | `vl_api_<msg>_t_calc_size` — computes actual message size including variable-length arrays (VLAs). Used by the dispatch engine to know how much data to read |
| `is_autoendian` | When set to 1, the dispatch loop automatically calls the registered `endian` handler on the message before invoking the handler callback. When 0, the handler (or caller) is responsible for its own byte-order conversion |

**Registration example — memclnt messages (static IDs):**

```c
vl_msg_api_set_handlers(VL_API_CONTROL_PING_REPLY + 1,  /* +1 for 1-based indexing */
                        "control_ping_reply",
                        vl_api_control_ping_reply_t_handler,
                        vl_noop_handler,                  /* cleanup */
                        vl_api_control_ping_reply_t_endian,
                        sizeof(vl_api_control_ping_reply_t),
                        1,                                /* traced */
                        vl_noop_handler,                  /* tojson — not generated */
                        vl_noop_handler,                  /* fromjson — not generated */
                        vl_api_control_ping_reply_t_calc_size);
```

**Registration example — plugin messages (dynamic IDs):**

```c
vl_msg_api_set_handlers(VL_API_SW_INTERFACE_DETAILS + interface_msg_id_base,
                        "sw_interface_details",
                        vl_api_sw_interface_details_t_handler,
                        vl_noop_handler,
                        vl_api_sw_interface_details_t_endian,
                        sizeof(vl_api_sw_interface_details_t),
                        1,
                        vl_noop_handler,
                        vl_noop_handler,
                        vl_api_sw_interface_details_t_calc_size);
```

Note: plugin handlers must be registered **after** `get_msg_id_bases()` has resolved `interface_msg_id_base`, since the ID computation depends on it. Base memclnt handlers can be registered earlier since their IDs are static.

**Step 4 — Wait for a result (`W` / `WR`):**

```c
W(ret);     /* standard: waits up to 1 second */
WR(ret);    /* retry variant: loops until result_ready or timeout */
```

Both macros poll `vam->result_ready`, which each reply handler sets via `set_reply_status()`:

```c
static void set_reply_status(int retval) {
    vam->retval = retval;
    vam->result_ready = 1;
}
```

The `W` macro implementation:

```c
#define W(ret)                                                  \
do {                                                            \
    f64 timeout = vat_time_now(vam) + 1.0;                      \
    ret = -99;                                                  \
    if (scm && scm->socket_enable)                              \
      vl_socket_client_read(5);                                 \
    while (vat_time_now(vam) < timeout) {                       \
        if (vam->result_ready == 1) {                           \
            ret = vam->retval;                                  \
            break;                                              \
        }                                                       \
        vat_suspend(vam->vlib_main, 1e-5);                      \
    }                                                           \
} while(0);
```

---

## The `control_ping` Synchronization Pattern

Because some API calls produce **multiple reply messages** (e.g., `sw_interface_dump` sends one `sw_if_details` per interface with no defined terminator), VPP uses `control_ping` as a **flush/fence**. The client sends a `control_ping` immediately after the request; VPP processes it in order and sends back `control_ping_reply` only after all preceding replies have been dispatched. The `W` then waits for that ping reply rather than counting individual detail messages.

```c
/* Example: dump interfaces, synchronize with ping */
__plugin_msg_base = interface_msg_id_base;
M(SW_INTERFACE_DUMP, mp);
S(mp);

__plugin_msg_base = memclnt_msg_id_base;
PING(NULL, mp_ping);   /* allocates a control_ping request */
S(mp_ping);
W(ret);                /* waits for control_ping_reply */
```

The `control_ping_reply` handler also decrements `control_pings_outstanding` on the socket client, which the socket layer uses to know when the stream is fully drained.

---

## Complete Example: Adding a BFD Session

```c
vat_main_t *vam = &vat_main;
vl_api_bfd_udp_add_t *mp;
int ret;

__plugin_msg_base = bfd_msg_id_base;
M(BFD_UDP_ADD, mp);

mp->sw_if_index        = htonl(sw_if_index);
mp->desired_min_tx     = htonl(desired_min_tx_usec);
mp->required_min_rx    = htonl(required_min_rx_usec);
mp->detect_mult        = detect_mult;
mp->is_authenticated   = 0;
/* fill local/peer addresses via helper */
vpp_to_vl_api_ip_addr(&mp->local_addr, &local_addr);
vpp_to_vl_api_ip_addr(&mp->peer_addr,  &peer_addr);

S(mp);
WR(ret);   /* BFD_UDP_ADD has a single reply, no ping needed */
```

`bfd_udp_add_reply` handler calls `set_reply_status(ntohl(mp->retval))`, which sets `vam->result_ready = 1` and `vam->retval` to `0` on success or a negative VPP error code on failure. `WR` then returns that value in `ret`.

---

## Complete Example: List All Interfaces

Listing all interfaces is a **dump** operation — VPP streams back one `sw_interface_details` reply per interface rather than a single reply, so a `control_ping` fence is required to know when the stream ends.

### Step 1: The reply handler

Before sending any request, a handler must be registered to collect each `sw_interface_details` message. This is done once during init in `vpp_ext_vpe_init()` via the X-macro pattern. The handler stores each interface name and its `sw_if_index` into two hash tables for later lookup:

```c
static void
vl_api_sw_interface_details_t_handler(vl_api_sw_interface_details_t *mp)
{
    vat_main_t *vam = &vat_main;

    /* Build a null-terminated copy of the interface name string */
    u8 *s = format(0, "%s%c", mp->interface_name, 0);

    /* name → sw_if_index */
    hash_set_mem(vam->sw_if_index_by_interface_name, s, ntohl(mp->sw_if_index));

    /* sw_if_index → name */
    hash_set(interface_name_by_sw_index, ntohl(mp->sw_if_index), s);
}
```

Note that all integer fields arriving from VPP are in **network byte order**, so `ntohl()` is applied before storing `sw_if_index`.

### Step 2: Send the dump request and wait

```c
int api_sw_interface_dump(vat_main_t *vam)
{
    vl_api_sw_interface_dump_t  *mp;
    vl_api_control_ping_t       *mp_ping;
    int ret;

    /*
     * Point __plugin_msg_base at the interface plugin's ID base so that
     * M() computes the correct _vl_msg_id for SW_INTERFACE_DUMP.
     */
    __plugin_msg_base = interface_msg_id_base;

    /* Allocate, zero, and stamp the request message */
    M(SW_INTERFACE_DUMP, mp);
    /*
     * sw_if_index = ~0 requests all interfaces.
     * The field is already 0 from clib_memset, and 0 / ~0 both mean "all"
     * in this API, so no extra field setup is needed here.
     */

    /* Transmit the request */
    S(mp);

    /*
     * SW_INTERFACE_DUMP produces N reply messages (one per interface).
     * There is no end-of-stream marker, so send a control_ping immediately
     * after. VPP will process it in order and send control_ping_reply only
     * after all sw_interface_details messages have been sent.
     * Switch __plugin_msg_base to the memclnt base for the ping message ID.
     */
    __plugin_msg_base = memclnt_msg_id_base;
    PING(NULL, mp_ping);
    S(mp_ping);

    /*
     * W() blocks until vam->result_ready == 1.
     * The control_ping_reply handler sets result_ready, so this returns
     * only after all sw_interface_details have already been processed
     * by vl_api_sw_interface_details_t_handler above.
     */
    W(ret);

    return ret;  /* 0 on success */
}
```

### Step 3: Read back the results

After `api_sw_interface_dump()` returns, the two hash tables are fully populated. `dump_interface_table()` iterates them to print every interface:

```c
static int dump_interface_table(vat_main_t *vam)
{
    hash_pair_t *p;

    hash_foreach_pair(p, vam->sw_if_index_by_interface_name, ({
        u8    *name     = (u8 *)  (p->key);
        u32    sw_index = (u32)   p->value[0];
        print(vam->ofp, "%-25s sw_if_index: %d", name, sw_index);
    }));

    return 0;
}
```

### Message flow diagram

```
Client                              VPP
  |                                  |
  |--- SW_INTERFACE_DUMP ----------->|
  |                                  |
  |<-- sw_interface_details (eth0) --|   handler: hash_set name→index
  |<-- sw_interface_details (eth1) --|   handler: hash_set name→index
  |<-- sw_interface_details (lo)   --|   handler: hash_set name→index
  |                                  |
  |--- CONTROL_PING ---------------->|
  |                                  |
  |<-- control_ping_reply -----------|   handler: set result_ready = 1
  |                                  |
  W() unblocks, ret = 0
```

---

## Key Data Structures: `vat_main_t` vs `api_main_t`

These two structs operate at **different layers** of the VPP client stack.

### `vat_main_t` — the API client session (`<vat/vat.h>`)

This is the **application-level** context. It belongs to your program and represents your active conversation with VPP. Key fields:

| Field | Purpose |
|---|---|
| `result_ready`, `retval`, `sw_if_index` | Volatile synchronization fields; handlers write these, `W()` polls them |
| `socket_client_main`, `socket_name` | Which socket transport to use |
| `my_client_index` | This client's handle, stamped into every outgoing message |
| `sw_if_index_by_interface_name` | Hash table built from `sw_interface_details` replies |
| `sw_if_subif_table` | Sub-interface metadata array |
| `clib_time` | Wall-clock used by `vat_time_now()` for `W()` timeout |
| `vlib_main` | VPP main thread context, needed by `vat_suspend()` |
| `async_mode` / `async_errors` | When set, handlers count errors instead of signalling `result_ready` |

There is one global instance: `vat_main`.

### `api_main_t` — the transport and dispatch engine (`<vlibapi/api_common.h>`)

This is the **infrastructure layer** underneath. It belongs to the VPP client library itself. Key fields:

| Field | Purpose |
|---|---|
| `msg_data`, `msg_id_by_name` | The handler dispatch table, populated by `vl_msg_api_set_handlers()` |
| `shmem_hdr`, `vlib_rp`, `vl_input_queue` | Raw shared-memory region and queue (used when not in socket mode) |
| `first_available_msg_id`, `msg_range_by_name`, `msg_ranges` | Dynamic plugin message ID allocation; queried by `vl_client_get_first_plugin_msg_id()` |
| `my_client_index` | Same value as `vat_main.my_client_index`, copied during connect |

There is one global instance: `api_main`, accessed via `vlibapi_get_main()`.

### Relationship

```
Your code
    │
    ▼
vat_main_t      ← you use this (M/S/W/PING, result_ready, hash tables)
    │
    ▼
api_main_t      ← library uses this (handler dispatch, SHM transport, plugin msg IDs)
    │
    ▼
VPP (socket or SHM)
```

In practice you touch `api_main_t` directly in only two places: during connect to copy `my_client_index`, and when calling `vl_client_get_first_plugin_msg_id()` which queries it internally. Everything else goes through `vat_main_t`.

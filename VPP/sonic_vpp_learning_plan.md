# SONiC-VPP Developer Learning Plan

A structured learning path for SONiC developers who want to contribute to sonic-vpp, especially implementing features that don't yet have VPP support.

---

## Prerequisites

Before starting, you should have:
- Basic understanding of SONiC architecture (orchagent, syncd, SAI)
- Familiarity with C/C++ programming
- Basic networking concepts (routing, switching, ACLs, tunnels)
- Linux command line experience

---

## Phase 1: Understanding the Full Architecture (Week 1-2)

### 1.1 SONiC Architecture Deep Dive

**Goal:** Understand how SONiC works end-to-end

**Learning Resources:**
- Read [SONiC Architecture](https://github.com/sonic-net/SONiC/wiki/Architecture)
- Study the data flow: CLI/Config → CONFIG_DB → orchagent → ASIC_DB → syncd → SAI → Hardware

**Hands-on:**
```bash
# Explore a running SONiC system
docker exec -it swss bash
redis-cli -n 0  # CONFIG_DB
redis-cli -n 1  # ASIC_DB
redis-cli -n 2  # COUNTERS_DB

# Watch configuration flow
redis-cli --csv --bigkeys
redis-cli monitor
```

**Key Concepts to Master:**
- Redis databases and their purposes
- SONiC container architecture
- Control plane vs data plane separation
- SAI (Switch Abstraction Interface) API

**Checkpoint:** Can you explain how adding an IP route flows from CLI to hardware?

### 1.2 SAI Interface Fundamentals

**Goal:** Understand SAI API structure and patterns

**Learning Resources:**
- Browse [SAI headers](https://github.com/opencomputeproject/SAI/tree/master/inc)
- Read `sai_route.h`, `sai_port.h`, `sai_acl.h` for common patterns
- Study [syncd_main.md](../sonic/sairedis/syncd_main.md) - SaiInterface hierarchy

**Key Concepts:**
- SAI object types and attributes
- CRUD operations (create, remove, set, get)
- Entry-based APIs (routes, neighbors, FDB) vs OID-based APIs (ports, VLANs)
- Bulk operations
- Statistics and counters

**Hands-on:**
```bash
# Explore SAI objects in ASIC_DB
redis-cli -n 1
> KEYS ASIC_STATE:SAI_OBJECT_TYPE_ROUTE_ENTRY*
> HGETALL "ASIC_STATE:SAI_OBJECT_TYPE_ROUTE_ENTRY:..."
> KEYS ASIC_STATE:SAI_OBJECT_TYPE_PORT:*
```

**Checkpoint:** Can you identify SAI attributes needed to create a route? An ACL rule?

### 1.3 VPP Deep Dive - From CLI to Binary API

**Goal:** Master VPP Binary API - the core interface for SONiC-VPP integration

Since you already know VPP CLI basics (interfaces, routing, tracing), we'll focus on how programs interact with VPP through the Binary API.

#### VPP Architecture Recap

```
┌─────────────────────────────────────────────┐
│  Client Applications (syncd, vppctl, etc)   │
└─────────────────────────────────────────────┘
              │
              │ API Client Library
              ▼
┌─────────────────────────────────────────────┐
│         VPP Binary API (vlib API)           │
│    (Shared Memory Message Passing)          │
└─────────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────────┐
│         VPP Main Thread                     │
│    - Process API messages                   │
│    - Call plugin handlers                   │
│    - Send replies                           │
└─────────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────────┐
│         VPP Worker Threads                  │
│    - Process packets via graph nodes        │
│    - L2/L3 forwarding                       │
└─────────────────────────────────────────────┘
```

**Key Insight:** When you run `vppctl set interface ip address...`, vppctl translates this into a Binary API message and sends it to VPP. In SONiC-VPP, `syncd` does the same thing!

#### 1.3.1 Understanding VPP Binary API

**What is Binary API?**
- Efficient message passing over shared memory
- Type-safe, versioned interface
- Each plugin defines its own API messages
- CLI is just a wrapper around Binary API

**API Message Types:**
1. **Request/Reply** - Client sends request, VPP sends one reply
2. **Dump/Details** - Client requests data, VPP sends multiple replies (like `show` commands)
3. **Events** - VPP sends unsolicited notifications (link up/down, etc)

**Hands-on: Explore API Definitions**

```bash
# In VPP source code
cd /path/to/vpp/src

# Find API definitions (.api files)
find . -name "*.api" | head -20
# Examples:
# ./vnet/interface.api       - Interface operations
# ./vnet/ip/ip.api           - IP routing
# ./plugins/acl/acl.api      - ACL operations
# ./vlib/vlib.api            - Core VPP APIs
```

**Example: `interface.api`**
```c
// Located in: vpp/src/vnet/interface.api

// Set interface state (up/down)
autoreply define sw_interface_set_flags
{
  u32 client_index;      // Set by client library
  u32 context;           // Request ID for matching replies
  vl_api_interface_index_t sw_if_index;  // Interface index
  vl_api_if_status_flags_t flags;        // UP=1, DOWN=0
};

// Add IP address to interface
autoreply define sw_interface_add_del_address
{
  u32 client_index;
  u32 context;
  vl_api_interface_index_t sw_if_index;
  bool is_add [default=true];
  bool del_all [default=false];
  vl_api_address_with_prefix_t prefix;
};
```

**What "autoreply" means:** VPP will automatically send a standardized reply with just `retval` (return code).

#### 1.3.2 API Message Structure

**Every API message has:**
1. **Message ID** - Identifies the message type
2. **Client Index** - Identifies the client connection
3. **Context** - Correlates requests with replies
4. **Payload** - Message-specific data

**Generated C Structures:**

When you build VPP, it generates C structures from `.api` files:

```c
// Auto-generated from interface.api
typedef struct _vl_api_sw_interface_set_flags
{
  u16 _vl_msg_id;                    // Message ID
  u32 client_index;                   // Your client connection
  u32 context;                        // Request ID
  vl_api_interface_index_t sw_if_index;  // uint32_t
  vl_api_if_status_flags_t flags;        // uint8_t
} __attribute__ ((packed)) vl_api_sw_interface_set_flags_t;

// The reply
typedef struct _vl_api_sw_interface_set_flags_reply
{
  u16 _vl_msg_id;
  u32 context;                        // Matches request context
  i32 retval;                         // Return code (0 = success)
} __attribute__ ((packed)) vl_api_sw_interface_set_flags_reply_t;
```

#### 1.3.3 How to Use Binary API in C

**Step 1: Connect to VPP**

```c
#include <vpp/api/vpe_msg_enum.h>
#include <vppinfra/mem.h>
#include <vlibapi/api.h>
#include <vlibmemory/api.h>

// Initialize VPP API client
int connect_to_vpp(const char *client_name)
{
    // Connect to VPP shared memory
    if (vl_client_connect_to_vlib("/vpe-api", client_name, 32) < 0) {
        fprintf(stderr, "Failed to connect to VPP\n");
        return -1;
    }
    
    // Get message base IDs for your plugin
    vl_api_msg_id_base_t base_msg_id;
    
    return 0;
}
```

**Step 2: Send a Request**

Let's bring interface UP (equivalent to `vppctl set interface state eth0 up`):

```c
#include <vnet/interface.api_types.h>
#include <vnet/interface.api_enum.h>

// Macros for message handling
#define M(T, mp) \
  do { \
    mp = vl_msg_api_alloc_as_if_client(sizeof(*mp)); \
    memset(mp, 0, sizeof(*mp)); \
    mp->_vl_msg_id = vl_msg_api_get_msg_index((u8 *) VL_API_##T##_CRC); \
    mp->client_index = am->my_client_index; \
    mp->context = 0xdeadbeef; \
  } while(0);

#define S(mp) vl_msg_api_send_shmem(am->vl_input_queue, (u8 *)&mp)

int set_interface_up(uint32_t sw_if_index)
{
    vl_api_sw_interface_set_flags_t *mp;
    
    // Allocate and initialize message
    M(SW_INTERFACE_SET_FLAGS, mp);
    
    // Fill in parameters
    mp->sw_if_index = htonl(sw_if_index);
    mp->flags = IF_STATUS_API_FLAG_ADMIN_UP;
    
    // Send message
    S(mp);
    
    // Wait for reply (simplified)
    // In real code, you'd register a callback
    
    return 0;
}
```

**Step 3: Receive Reply**

```c
// Reply handler callback
static void vl_api_sw_interface_set_flags_reply_t_handler(
    vl_api_sw_interface_set_flags_reply_t *mp)
{
    if (mp->retval != 0) {
        fprintf(stderr, "Error: retval=%d\n", ntohl(mp->retval));
    } else {
        printf("Interface set successfully\n");
    }
}

// Register handler during initialization
void setup_message_handlers(void)
{
    vl_msg_api_set_handlers(
        VL_API_SW_INTERFACE_SET_FLAGS_REPLY,
        "sw_interface_set_flags_reply",
        vl_api_sw_interface_set_flags_reply_t_handler,
        vl_noop_handler,
        vl_api_sw_interface_set_flags_reply_t_endian,
        vl_api_sw_interface_set_flags_reply_t_print,
        sizeof(vl_api_sw_interface_set_flags_reply_t),
        1  /* do_it */
    );
}
```

#### 1.3.4 Practical Example: Add IP Route via Binary API

This is what happens when SONiC-VPP adds a route!

**API Definition (`ip.api`):**
```c
autoreply define ip_route_add_del
{
  u32 client_index;
  u32 context;
  bool is_add [default=true];
  bool is_multipath;
  vl_api_ip_route_t route;  // Contains prefix + paths
};

typedef ip_route
{
  u32 table_id;
  u32 stats_index;
  vl_api_prefix_t prefix;
  u8 n_paths;
  vl_api_fib_path_t paths[n_paths];
};
```

**Implementation (similar to SaiVppXlate.c):**

```c
int add_ip_route(const char *prefix_str, const char *nexthop_str)
{
    vl_api_ip_route_add_del_t *mp;
    ip_address_t prefix_addr, nexthop_addr;
    u8 prefix_len;
    
    // Parse prefix (e.g., "192.168.1.0/24")
    ip_address_decode(prefix_str, &prefix_addr);
    prefix_len = 24;  // Parse from string
    
    // Parse nexthop
    ip_address_decode(nexthop_str, &nexthop_addr);
    
    // Allocate message
    // M2 is like M but allows variable-length arrays
    M2(IP_ROUTE_ADD_DEL, sizeof(vl_api_fib_path_t), mp);
    
    // Fill route info
    mp->is_add = 1;
    mp->is_multipath = 0;
    
    // Fill prefix
    mp->route.prefix.address_length = prefix_len;
    memcpy(&mp->route.prefix.address, &prefix_addr, sizeof(prefix_addr));
    
    // Fill path (nexthop)
    mp->route.n_paths = 1;
    mp->route.paths[0].sw_if_index = ~0;  // Invalid = use nexthop lookup
    memcpy(&mp->route.paths[0].nh.address, &nexthop_addr, sizeof(nexthop_addr));
    mp->route.paths[0].proto = FIB_API_PATH_NH_PROTO_IP4;
    
    // Send
    S(mp);
    
    return 0;
}
```

**This is EXACTLY what happens in `SaiVppXlate.c:ip_route_add_del()`!**

#### 1.3.5 Dump Operations (Equivalent to "show" commands)

When you run `vppctl show interface`, it uses a dump operation:

```c
// Request all interfaces
void dump_interfaces(void)
{
    vl_api_sw_interface_dump_t *mp;
    
    M(SW_INTERFACE_DUMP, mp);
    
    // Request all interfaces
    mp->sw_if_index = ~0;  // Special value for "all"
    mp->name_filter_valid = 0;
    
    S(mp);
}

// Handler receives MULTIPLE replies (one per interface)
static void vl_api_sw_interface_details_t_handler(
    vl_api_sw_interface_details_t *mp)
{
    printf("Interface: %s\n", mp->interface_name);
    printf("  sw_if_index: %u\n", ntohl(mp->sw_if_index));
    printf("  admin_up: %d\n", mp->flags & IF_STATUS_API_FLAG_ADMIN_UP);
    printf("  link_up: %d\n", mp->flags & IF_STATUS_API_FLAG_LINK_UP);
    // ... more fields
}
```

#### 1.3.6 How SONiC-VPP Uses Binary API

**The Complete Flow:**

1. **Initialization** (`SaiVppXlate.c:vpp_connect()`):
```c
int vpp_connect(void)
{
    // Connect to VPP shared memory
    if (vac_connect("saivpp", NULL, NULL, 0) != 0) {
        return -1;
    }
    
    // Register message handlers
    setup_message_handlers();
    
    return 0;
}
```

2. **SAI → VPP API** (when orchagent adds a route):
```
orchagent → ASIC_DB → syncd → saivpp::Sai::create()
  → SwitchStateBase::addIpRoute()
  → vppxlate::ip_route_add_del()    ← YOU ARE HERE
  → VPP Binary API message
  → VPP processes route
  → VPP reply
  → return SAI_STATUS_SUCCESS
```

3. **VPP Event → SONiC** (when link goes down):
```
VPP detects link down
  → VPP sends sw_interface_event notification
  → vl_api_sw_interface_event_t_handler()    ← In SaiVppXlate.c
  → Queue event
  → vppProcessEvents() thread picks it up
  → asyncIntfStateUpdate()
  → Send SAI notification to orchagent
```

#### 1.3.7 Hands-on Practice

**Exercise 1: Connect to VPP and List Interfaces**

```c
// mini_vpp_client.c
#include <vppinfra/clib.h>
#include <vlibapi/api.h>
#include <vlibmemory/api.h>

int main(int argc, char **argv)
{
    // Connect
    if (vl_client_connect_to_vlib("/vpe-api", "mini_client", 32) < 0) {
        printf("Failed to connect\n");
        return 1;
    }
    
    printf("Connected to VPP!\n");
    
    // Send dump request
    vl_api_sw_interface_dump_t *mp;
    M(SW_INTERFACE_DUMP, mp);
    mp->sw_if_index = ~0;
    S(mp);
    
    // Wait for replies...
    sleep(1);
    
    vl_client_disconnect_from_vlib();
    return 0;
}
```

**Exercise 2: Study Real SONiC-VPP Code**

```bash
# Read the actual implementation
cd sonic-buildimage/platform/vpp/saivpp/src/vppxlate

# Route operations
grep -A 50 "ip_route_add_del" SaiVppXlate.c

# Interface operations  
grep -A 50 "interface_set_state" SaiVppXlate.c

# Event handling
grep -A 50 "sw_interface_event" SaiVppXlate.c
```

**Exercise 3: Trace API Messages**

```bash
# In VPP
vppctl api trace on
vppctl api trace save /tmp/api_trace.txt

# In SONiC, add a route
config route add prefix 10.1.1.0/24 nexthop 192.168.1.1

# Check trace
cat /tmp/api_trace.txt
# You'll see the exact API messages sent!
```

#### 1.3.8 Key Takeaways

✅ **CLI is just a wrapper** - Every vppctl command becomes a Binary API call  
✅ **SONiC-VPP uses the same API** - `SaiVppXlate.c` is a SAI-to-VPP-API translator  
✅ **Messages are typed** - Each API has request/reply structures  
✅ **Shared memory is fast** - No socket overhead, zero-copy where possible  
✅ **Events enable async** - VPP can notify clients of link state changes  

#### 1.3.9 Resources

**VPP API Documentation:**
- [VPP API Guide](https://s3-docs.fd.io/vpp/25.06/developer/corefeatures/api/index.html)
- [API Message Format](https://wiki.fd.io/view/VPP/Python_API)

**Source Code to Study:**
```bash
# In VPP source
vpp/src/vnet/interface.api       # Interface APIs
vpp/src/vnet/ip/ip.api           # IP/routing APIs
vpp/src/vnet/l2/l2.api           # L2/switching APIs
vpp/src/plugins/acl/acl.api      # ACL APIs

# In SONiC-VPP
platform/vpp/saivpp/src/vppxlate/SaiVppXlate.c   # All VPP API calls
platform/vpp/saivpp/src/vppxlate/SaiVppXlate.h   # VPP data structures
```

**Checkpoint:** 
- ✅ Can you explain the difference between VPP CLI and Binary API?
- ✅ Can you trace a vppctl command to its Binary API message?
- ✅ Do you understand M/S/W macros and what they do?
- ✅ Can you find the API definition for a VPP feature?
- ✅ Ready to implement your first SAI→VPP translation?

---

## Phase 2: SONiC-VPP Integration Architecture (Week 3)

### 2.1 Study the Integration Points

**Goal:** Understand how SONiC and VPP communicate

**Key Files to Read:**
```
sonic-buildimage/platform/vpp/
├── saivpp/
│   ├── src/Sai.cpp                    # Entry point, creates VirtualSwitchSaiInterface
│   ├── src/Sai.h                      # saivpp::Sai declaration
│   └── src/vppxlate/
│       ├── SaiVppXlate.h              # C structures for SAI→VPP translation
│       └── SaiVppXlate.c              # VPP Binary API wrappers
```

**Study the Layering:**
```
orchagent → ASIC_DB → syncd → saivpp::Sai → Meta → VirtualSwitchSaiInterface
            → SwitchVpp → SaiVppXlate → VPP Binary API → VPP Process
```

**Read These Documents:**
- [SONICVPP-HLD.md](../../sonic-buildimage/platform/vpp/docs/HLD/SONICVPP-HLD.md)
- [vpp_sonic_intergation_overview.md](./vpp_sonic_intergation_overview.md)
- [syncd_main.md](../sonic/sairedis/syncd_main.md) - VPP SONiC Architecture section

### 2.2 VPP Binary API Mastery - Deep Dive

**Goal:** Become proficient at calling VPP features from C code (the core skill for SONiC-VPP development)

Now that you understand the basics, let's go deeper into practical API usage patterns you'll use daily.

#### 2.2.1 The VPP API Workflow

**Complete Example: Adding an IP Interface**

This is what happens when SONiC adds an IP address to `Ethernet0`:

**1. Find the API definition:**
```bash
# Search VPP source for IP address APIs
cd /path/to/vpp
grep -r "interface_add_del_address" src/vnet/ip/*.api

# Found in: src/vnet/ip/ip.api
```

**2. Read the API contract:**
```c
// From vnet/ip/ip.api
autoreply define sw_interface_add_del_address
{
  u32 client_index;
  u32 context;
  vl_api_interface_index_t sw_if_index;   // Which interface
  bool is_add [default=true];              // Add or delete?
  bool del_all [default=false];            // Delete all addresses?
  vl_api_address_with_prefix_t prefix;     // IP address + prefix length
};

// The prefix structure
typedef address_with_prefix
{
  vl_api_address_t address;   // IP address (v4 or v6)
  u8 len;                     // Prefix length (0-32 for v4, 0-128 for v6)
};

typedef address
{
  vl_api_address_family_t af;  // ADDRESS_IP4 or ADDRESS_IP6
  vl_api_address_union_t un;   // Union of ip4/ip6
};
```

**3. Implement in SaiVppXlate.c style:**

```c
// Structure to pass from C++ to C
typedef struct _vpp_ip_addr_t {
    uint32_t sw_if_index;
    uint8_t  is_ipv6;
    uint8_t  addr[16];        // IPv4 uses first 4 bytes
    uint8_t  prefix_len;
} vpp_ip_addr_t;

// VPP API wrapper
int interface_ip_address_add_del(vpp_ip_addr_t *ip_addr, bool is_add)
{
    vl_api_sw_interface_add_del_address_t *mp;
    int ret = 0;
    
    // Mutex lock (VPP API is NOT thread-safe)
    VPP_LOCK();
    
    // Allocate message
    M(SW_INTERFACE_ADD_DEL_ADDRESS, mp);
    
    // Fill parameters
    mp->sw_if_index = htonl(ip_addr->sw_if_index);
    mp->is_add = is_add;
    mp->del_all = 0;
    
    // Fill address
    if (ip_addr->is_ipv6) {
        mp->prefix.address.af = ADDRESS_IP6;
        memcpy(mp->prefix.address.un.ip6, ip_addr->addr, 16);
    } else {
        mp->prefix.address.af = ADDRESS_IP4;
        memcpy(mp->prefix.address.un.ip4, ip_addr->addr, 4);
    }
    mp->prefix.len = ip_addr->prefix_len;
    
    // Send message
    S(mp);
    
    // Wait for reply (blocks until VPP responds)
    W(ret);
    
    VPP_UNLOCK();
    
    if (ret != 0) {
        fprintf(stderr, "VPP API error: %d\n", ret);
        return -1;
    }
    
    return 0;
}
```

**4. Call from SAI layer (SwitchStateBase):**

```cpp
// In SwitchStateBasePort.cpp
sai_status_t SwitchStateBase::set(
    _In_ sai_object_type_t objectType,
    _In_ sai_object_id_t objectId,
    _In_ const sai_attribute_t *attr)
{
    if (objectType == SAI_OBJECT_TYPE_ROUTER_INTERFACE &&
        attr->id == SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS) {
        
        // Get port info
        auto port_info = m_router_interfaces.find(objectId);
        uint32_t sw_if_index = port_info->second.vpp_sw_if_index;
        
        // Convert SAI IP to VPP format
        vpp_ip_addr_t vpp_addr;
        vpp_addr.sw_if_index = sw_if_index;
        vpp_addr.is_ipv6 = (attr->value.ipaddr.addr_family == SAI_IP_ADDR_FAMILY_IPV6);
        
        if (vpp_addr.is_ipv6) {
            memcpy(vpp_addr.addr, attr->value.ipaddr.addr.ip6, 16);
            vpp_addr.prefix_len = 128;
        } else {
            memcpy(vpp_addr.addr, &attr->value.ipaddr.addr.ip4, 4);
            vpp_addr.prefix_len = 32;
        }
        
        // Call VPP
        if (interface_ip_address_add_del(&vpp_addr, true) != 0) {
            return SAI_STATUS_FAILURE;
        }
        
        return SAI_STATUS_SUCCESS;
    }
    
    // ... other attributes
}
```

#### 2.2.2 Common API Patterns

**Pattern 1: Simple Request-Reply (Most Common)**

Used for: Creating objects, setting state, etc.

```c
int do_simple_operation(void)
{
    vl_api_some_operation_t *mp;
    int ret;
    
    VPP_LOCK();
    M(SOME_OPERATION, mp);
    
    // Fill parameters
    mp->param1 = value1;
    mp->param2 = value2;
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

**Pattern 2: Variable-Length Arrays**

Used for: Routes with multiple nexthops, ACL rules with multiple matches

```c
int add_route_with_ecmp(int num_nexthops)
{
    vl_api_ip_route_add_del_t *mp;
    int ret;
    
    // Calculate extra size needed
    size_t extra = (num_nexthops - 1) * sizeof(vl_api_fib_path_t);
    
    VPP_LOCK();
    
    // M2 allocates extra space
    M2(IP_ROUTE_ADD_DEL, extra, mp);
    
    mp->route.n_paths = num_nexthops;
    
    // Fill each path
    for (int i = 0; i < num_nexthops; i++) {
        mp->route.paths[i].sw_if_index = nexthop_sw_if[i];
        mp->route.paths[i].weight = 1;  // Equal cost
        // ... more fields
    }
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

**Pattern 3: Dump Operations (Retrieving State)**

Used for: Getting statistics, listing objects

```c
// Global to collect results
static int interface_count = 0;
static vl_api_sw_interface_details_t interfaces[256];

// Reply handler (called for EACH interface)
static void vl_api_sw_interface_details_t_handler(
    vl_api_sw_interface_details_t *mp)
{
    if (interface_count < 256) {
        memcpy(&interfaces[interface_count], mp, sizeof(*mp));
        interface_count++;
    }
}

// Dump request
int get_all_interfaces(void)
{
    vl_api_sw_interface_dump_t *mp;
    
    interface_count = 0;
    
    VPP_LOCK();
    M(SW_INTERFACE_DUMP, mp);
    
    mp->sw_if_index = ~0;  // All interfaces
    mp->name_filter_valid = 0;
    
    S(mp);
    
    // Wait for VPP to finish sending all replies
    // This is tricky - need to use control ping
    vl_api_control_ping_t *mp_ping;
    M(CONTROL_PING, mp_ping);
    S(mp_ping);
    
    // Wait for control ping reply (signals end of dump)
    // W macro handles this
    
    VPP_UNLOCK();
    
    printf("Found %d interfaces\n", interface_count);
    return 0;
}
```

**Pattern 4: Async Events (Notifications)**

Used for: Link state changes, BFD session state, etc.

```c
// Event handler (VPP calls this when link state changes)
static void vl_api_sw_interface_event_t_handler(
    vl_api_sw_interface_event_t *mp)
{
    uint32_t sw_if_index = ntohl(mp->sw_if_index);
    bool link_up = mp->flags & IF_STATUS_API_FLAG_LINK_UP;
    
    printf("Interface %u link is %s\n", 
           sw_if_index, link_up ? "UP" : "DOWN");
    
    // In SONiC-VPP, this queues an event that
    // eventually becomes a SAI notification
    vpp_ev_enqueue(VPP_INTF_LINK_STATUS, sw_if_index, link_up);
}

// Enable event notifications (called during init)
int enable_interface_events(void)
{
    vl_api_want_interface_events_t *mp;
    int ret;
    
    VPP_LOCK();
    M(WANT_INTERFACE_EVENTS, mp);
    
    mp->enable_disable = 1;  // Enable
    mp->pid = getpid();
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

#### 2.2.3 Real SONiC-VPP Examples

Let's trace real operations:

**Example 1: Port Admin State Change**

```
SONiC CLI: config interface startup Ethernet0

↓ orchagent updates ASIC_DB

↓ syncd calls SAI: set_port_attribute(port_oid, SAI_PORT_ATTR_ADMIN_STATE, true)

↓ SwitchStateBase::set() in SwitchStateBasePort.cpp:
  - Find port_oid → vpp_sw_if_index mapping
  - Call interface_set_state(sw_if_index, true)

↓ SaiVppXlate.c:interface_set_state():
```

```c
int interface_set_state(uint32_t sw_if_index, bool is_up)
{
    vl_api_sw_interface_set_flags_t *mp;
    int ret;
    
    VPP_LOCK();
    M(SW_INTERFACE_SET_FLAGS, mp);
    
    mp->sw_if_index = htonl(sw_if_index);
    mp->flags = is_up ? IF_STATUS_API_FLAG_ADMIN_UP : 0;
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

**Example 2: Adding a Static Route**

```
SONiC CLI: config route add prefix 192.168.1.0/24 nexthop 10.0.0.1

↓ routeorch updates ASIC_DB

↓ syncd calls SAI: create_route_entry(route, attrs[])

↓ SwitchStateBase::create() in SwitchStateBaseRoute.cpp:
  - Extract prefix (192.168.1.0/24)
  - Extract nexthop (10.0.0.1)
  - Convert to vpp_ip_route_t structure
  - Call ip_route_add_del(route, true)

↓ SaiVppXlate.c:ip_route_add_del():
```

```c
int ip_route_add_del(vpp_ip_route_t *ip_route, bool is_add)
{
    vl_api_ip_route_add_del_t *mp;
    int ret;
    
    VPP_LOCK();
    
    // Allocate for one nexthop
    M2(IP_ROUTE_ADD_DEL, sizeof(vl_api_fib_path_t), mp);
    
    mp->is_add = is_add;
    mp->is_multipath = 0;
    
    // Fill prefix
    if (ip_route->is_ipv6) {
        mp->route.prefix.address.af = ADDRESS_IP6;
        memcpy(mp->route.prefix.address.un.ip6, 
               ip_route->prefix_addr, 16);
    } else {
        mp->route.prefix.address.af = ADDRESS_IP4;
        memcpy(mp->route.prefix.address.un.ip4, 
               ip_route->prefix_addr, 4);
    }
    mp->route.prefix.len = ip_route->prefix_len;
    
    // Fill nexthop path
    mp->route.n_paths = 1;
    mp->route.paths[0].sw_if_index = ~0;  // Use nexthop IP
    mp->route.paths[0].type = FIB_API_PATH_TYPE_NORMAL;
    
    if (ip_route->is_ipv6) {
        mp->route.paths[0].proto = FIB_API_PATH_NH_PROTO_IP6;
        memcpy(mp->route.paths[0].nh.address.ip6, 
               ip_route->nexthop_addr, 16);
    } else {
        mp->route.paths[0].proto = FIB_API_PATH_NH_PROTO_IP4;
        memcpy(mp->route.paths[0].nh.address.ip4, 
               ip_route->nexthop_addr, 4);
    }
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

#### 2.2.4 Debugging VPP API Issues

**Problem 1: Wrong Message ID**

```bash
# Symptom: VPP doesn't respond or responds with error

# Solution: Check message ID registration
vppctl api trace on
# Do operation
vppctl api trace save /tmp/api.trace

# Check trace file
cat /tmp/api.trace | grep -A 5 "your_message"
# Verify _vl_msg_id is correct
```

**Problem 2: Network Byte Order Issues**

```c
// WRONG - will cause weird behavior
mp->sw_if_index = sw_if_index;  // If sw_if_index > 255

// CORRECT - VPP expects network byte order
mp->sw_if_index = htonl(sw_if_index);

// When receiving
uint32_t sw_if_index = ntohl(mp->sw_if_index);
```

**Problem 3: Memory/Thread Safety**

```c
// WRONG - VPP API is NOT thread-safe
vl_api_some_call_t *mp;
M(SOME_CALL, mp);
mp->param = value;
S(mp);

// CORRECT - Always lock
VPP_LOCK();
vl_api_some_call_t *mp;
M(SOME_CALL, mp);
mp->param = value;
S(mp);
W(ret);
VPP_UNLOCK();
```

**Problem 4: Understanding Error Codes**

```c
W(ret);  // ret is the VPP error code

// Common error codes (from vnet/api_errno.h)
// 0 = Success
// -1 = VNET_API_ERROR_UNSPECIFIED
// -2 = VNET_API_ERROR_NO_SUCH_ENTRY
// -3 = VNET_API_ERROR_INVALID_VALUE
// -4 = VNET_API_ERROR_INVALID_VALUE_2
// etc.

if (ret != 0) {
    // Log the specific error
    SWSS_LOG_ERROR("VPP API failed with code: %d", ret);
}
```

#### 2.2.5 Advanced Topics

**Batching Operations:**

```c
// For performance, batch multiple operations
void add_multiple_routes(vpp_ip_route_t routes[], int count)
{
    VPP_LOCK();
    
    for (int i = 0; i < count; i++) {
        vl_api_ip_route_add_del_t *mp;
        M2(IP_ROUTE_ADD_DEL, sizeof(vl_api_fib_path_t), mp);
        
        // Fill message
        // ...
        
        S(mp);
        // Don't Wait (W) yet - queue all messages
    }
    
    // Now wait for all replies
    // (In practice, you'd use a more sophisticated approach)
    
    VPP_UNLOCK();
}
```

**Handling Timeouts:**

```c
// Set API timeout (default is 5 seconds)
vac_set_timeout(10);  // 10 seconds

// For operations that might take longer
VPP_LOCK();
M(SLOW_OPERATION, mp);
S(mp);

// Custom wait with timeout check
int ret = wait_for_reply_with_timeout(5000);  // 5 sec

VPP_UNLOCK();
```

#### 2.2.6 Hands-on Exercises

**Exercise 1: Implement ARP Entry Add**

Study VPP API for IP neighbor (ARP):
```bash
cat vpp/src/vnet/ip-neighbor/ip_neighbor.api
```

Implement:
```c
int neighbor_add_del(uint32_t sw_if_index, 
                     const uint8_t ip[4],
                     const uint8_t mac[6],
                     bool is_add);
```

**Exercise 2: Get Interface Statistics**

Implement a function to retrieve interface counters:
```c
typedef struct {
    uint64_t rx_packets;
    uint64_t rx_bytes;
    uint64_t tx_packets;
    uint64_t tx_bytes;
    uint64_t drops;
} interface_stats_t;

int get_interface_stats(uint32_t sw_if_index, interface_stats_t *stats);
```

Hint: Look at `vnet/interface.api` - `vnet_get_sw_interface_stats`

**Exercise 3: Enable/Disable Port Mirroring (SPAN)**

```c
// Set interface A to mirror traffic to interface B
int span_enable(uint32_t src_sw_if_index, 
                uint32_t dst_sw_if_index,
                bool direction_rx,  // Mirror RX traffic
                bool direction_tx); // Mirror TX traffic
```

Find the API in `vnet/span/span.api`

#### 2.2.7 Resources

**Essential Reading:**
```bash
# VPP API definitions (your reference manual)
vpp/src/vnet/interface.api          # Interfaces
vpp/src/vnet/ip/ip.api              # IP/Routing
vpp/src/vnet/ip-neighbor/ip_neighbor.api  # ARP/ND
vpp/src/vnet/l2/l2.api              # L2/Switching
vpp/src/plugins/acl/acl.api         # ACLs

# SONiC-VPP reference implementations
platform/vpp/saivpp/src/vppxlate/SaiVppXlate.c   # ALL VPP API calls
platform/vpp/saivpp/src/vppxlate/SaiVppXlate.h   # Data structures
```

**Online Documentation:**
- [VPP Binary API Guide](https://s3-docs.fd.io/vpp/25.06/developer/corefeatures/api/index.html)
- [API Type Definitions](https://s3-docs.fd.io/vpp/25.06/api/)

**Checkpoint:**
- ✅ Can you read a `.api` file and understand the message structure?
- ✅ Can you implement a VPP API wrapper function in C?
- ✅ Do you understand M/M2/S/W macros and when to use each?
- ✅ Can you trace a SAI call through to VPP API in SONiC-VPP code?
- ✅ Can you handle VPP API errors and debug issues?
- ✅ Ready to implement SAI features with VPP backend!

### 2.3 Study SwitchStateBase Implementation

**Goal:** Understand the SAI→VPP translation layer

**Key Files:**
```
platform/vpp/saivpp/src/
├── SwitchStateBase.h              # Base class for switch state
├── SwitchStateBaseRoute.cpp       # Route handling
├── SwitchStateBasePort.cpp        # Port handling
├── SwitchStateBaseAcl.cpp         # ACL handling
└── SwitchStateBaseUtils.cpp       # Helper functions
```

**Study Pattern:**
```cpp
// SAI API → SwitchStateBase method → VPP translation → VPP API

// Example: create_route_entry
sai_status_t SwitchStateBase::create(
    _In_ const sai_route_entry_t *route_entry,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    // 1. Extract SAI attributes
    auto nexthop = get_attribute(SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID);
    
    // 2. Call internal add method
    return addIpRoute(route_entry, nexthop);
}

sai_status_t SwitchStateBase::addIpRoute(...)
{
    // 3. Convert to VPP structure
    vpp_ip_route_t ip_route;
    create_route_prefix_entry(&ip_route.prefix, route_entry);
    
    // 4. Call VPP API
    return (ip_route_add_del(&ip_route, true) == 0) 
        ? SAI_STATUS_SUCCESS : SAI_STATUS_FAILURE;
}
```

**Checkpoint:** Can you trace a complete SAI API call to VPP for routes and ports?

---

## Phase 3: Development Environment Setup (Week 4)

### 3.1 Build SONiC-VPP from Source

**Setup Steps:**
```bash
# Clone repositories
git clone https://github.com/sonic-net/sonic-buildimage.git
cd sonic-buildimage
git checkout <vpp-branch>

# Build VPP platform
make init
make configure PLATFORM=vpp
make target/sonic-vpp.bin

# Or build docker image only
make target/docker-syncd-vpp.gz
```

**Alternative - Development Container:**
```bash
# For faster iteration, build in container
make sonic-slave-bash
# Inside container, build components individually
```

### 3.2 Setup Development Workflow

**Recommended IDE Setup:**
- VS Code with C/C++ extensions
- Clangd for code navigation
- Remote development into SONiC VM

**Code Navigation Tips:**
```bash
# Generate compile_commands.json for IDE
bear -- make

# Use ctags or cscope for large codebases
ctags -R platform/vpp/saivpp/
```

### 3.3 Setup Test Environment

**Option A: VM-based (Recommended for beginners)**
```bash
# Use KVM/QEMU
./platform/vpp/test/launch.sh
```

**Option B: Docker-based (Faster iteration)**
```bash
# Run syncd-vpp container standalone
docker run -it --privileged sonic-syncd-vpp bash
```

**Checkpoint:** Can you build sonic-vpp image and boot it in a VM?

---

## Phase 4: Implement Your First Feature (Week 5-7)

### 4.1 Choose a Feature to Implement

**Good Starter Features:**
- Simple features with clear VPP equivalents
- Features with limited dependencies

**Feature Complexity Levels:**

**Level 1 (Beginner):**
- ✅ Port enable/disable (already exists - study this first!)
- ✅ Basic routing (already exists - study this first!)
- ⚠️ Add missing port statistics
- ⚠️ Add missing interface attributes

**Level 2 (Intermediate):**
- ⚠️ Port mirroring (SPAN/ERSPAN) - VPP has mirror support
- ⚠️ FDB flush by VLAN
- ⚠️ Storm control (rate limiting)
- ⚠️ ECN configuration

**Level 3 (Advanced):**
- ⚠️ WRED (Weighted Random Early Detection)
- ⚠️ Scheduler configuration
- ⚠️ TAM (Telemetry and Monitoring)
- ⚠️ MACSEC

**Check Current Support:**
```bash
# See what's implemented
grep -r "SAI_OBJECT_TYPE_" platform/vpp/saivpp/src/
grep "TODO\|FIXME\|not supported" platform/vpp/saivpp/src/

# Check VPP capabilities
vppctl show api
vppctl show plugins
```

### 4.2 Implementation Step-by-Step

Let's walk through implementing a hypothetical feature: **Port Mirroring (SPAN)**

#### Step 1: Study SAI API
```bash
# Read SAI header
cat SAI/inc/saimirror.h
```

Key SAI objects:
- `SAI_OBJECT_TYPE_MIRROR_SESSION`
- Attributes: `SAI_MIRROR_SESSION_ATTR_TYPE`, `SAI_MIRROR_SESSION_ATTR_MONITOR_PORT`

#### Step 2: Study VPP Capability
```bash
vppctl help span
vppctl help show span

# Test VPP manually
vppctl set interface span GigabitEthernet0/0/0 destination GigabitEthernet0/0/1 both
vppctl show span
```

Find VPP Binary API:
```bash
# Look in VPP source
grep -r "span_add_del" /path/to/vpp/src/vnet/span/
# Find: vl_api_sw_interface_span_enable_disable_t
```

#### Step 3: Design the Translation

**Mapping:**
| SAI Concept | VPP Concept |
|-------------|-------------|
| Mirror session (OID) | SPAN configuration |
| Source port | Interface being monitored |
| Destination port | Destination interface |
| Direction (ingress/egress/both) | rx/tx/both flag |

**State to Maintain:**
```cpp
// In SwitchStateBase.h
struct MirrorSessionInfo {
    sai_object_id_t session_id;
    sai_object_id_t monitor_port;
    uint32_t monitored_ports;  // Count
    // VPP-specific data
    std::vector<uint32_t> vpp_src_sw_if_indices;
    uint32_t vpp_dst_sw_if_index;
};

std::map<sai_object_id_t, MirrorSessionInfo> m_mirror_sessions;
```

#### Step 4: Implement in VPP Translation Layer

**File: `vppxlate/SaiVppXlate.h`**
```c
// Add structure for mirror config
typedef struct _vpp_mirror_t {
    uint32_t src_sw_if_index;
    uint32_t dst_sw_if_index;
    uint8_t state;  // rx=1, tx=2, both=3
} vpp_mirror_t;

// Add function declaration
int span_enable_disable(vpp_mirror_t *mirror, bool is_enable);
```

**File: `vppxlate/SaiVppXlate.c`**
```c
int span_enable_disable(vpp_mirror_t *mirror, bool is_enable)
{
    vl_api_sw_interface_span_enable_disable_t *mp;
    int ret = 0;
    
    M2(VL_API_SW_INTERFACE_SPAN_ENABLE_DISABLE, 0, mp);
    
    VPP_LOCK();
    
    mp->sw_if_index_from = htonl(mirror->src_sw_if_index);
    mp->sw_if_index_to = htonl(mirror->dst_sw_if_index);
    mp->state = mirror->state;
    mp->enable = is_enable;
    
    S(mp);
    W(ret);
    
    VPP_UNLOCK();
    
    return ret;
}
```

#### Step 5: Implement SAI Interface

**File: `SwitchStateBaseMirror.cpp` (new file)**
```cpp
#include "SwitchStateBase.h"
#include "vppxlate/SaiVppXlate.h"

sai_status_t SwitchStateBase::create(
    _In_ sai_object_type_t objectType,
    _Out_ sai_object_id_t* objectId,
    _In_ sai_object_id_t switchId,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();
    
    if (objectType != SAI_OBJECT_TYPE_MIRROR_SESSION) {
        return SAI_STATUS_NOT_SUPPORTED;
    }
    
    // 1. Extract SAI attributes
    auto attr_type = sai_metadata_get_attr_by_id(
        SAI_MIRROR_SESSION_ATTR_TYPE, attr_count, attr_list);
    auto attr_monitor_port = sai_metadata_get_attr_by_id(
        SAI_MIRROR_SESSION_ATTR_MONITOR_PORT, attr_count, attr_list);
        
    if (!attr_monitor_port) {
        SWSS_LOG_ERROR("Missing monitor port");
        return SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING;
    }
    
    // 2. Validate (SPAN only, not ERSPAN yet)
    if (attr_type && attr_type->value.s32 != SAI_MIRROR_SESSION_TYPE_LOCAL) {
        SWSS_LOG_ERROR("Only local SPAN supported");
        return SAI_STATUS_NOT_SUPPORTED;
    }
    
    // 3. Create object ID
    *objectId = create_object_id(SAI_OBJECT_TYPE_MIRROR_SESSION, switchId);
    
    // 4. Store state
    MirrorSessionInfo info;
    info.session_id = *objectId;
    info.monitor_port = attr_monitor_port->value.oid;
    info.monitored_ports = 0;
    
    // Convert monitor port OID to VPP sw_if_index
    auto monitor_port_info = m_ports.find(info.monitor_port);
    if (monitor_port_info == m_ports.end()) {
        SWSS_LOG_ERROR("Monitor port not found");
        return SAI_STATUS_INVALID_PARAMETER;
    }
    info.vpp_dst_sw_if_index = monitor_port_info->second.vpp_sw_if_index;
    
    m_mirror_sessions[*objectId] = info;
    
    SWSS_LOG_NOTICE("Created mirror session %s", 
        sai_serialize_object_id(*objectId).c_str());
    
    return SAI_STATUS_SUCCESS;
}

sai_status_t SwitchStateBase::set(
    _In_ sai_object_type_t objectType,
    _In_ sai_object_id_t portId,
    _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();
    
    // Handle setting mirror session on a port
    if (objectType == SAI_OBJECT_TYPE_PORT &&
        attr->id == SAI_PORT_ATTR_INGRESS_MIRROR_SESSION) {
        
        return setPortMirrorSession(portId, attr->value.objlist, true);
    }
    
    // ... other attributes
}

sai_status_t SwitchStateBase::setPortMirrorSession(
    _In_ sai_object_id_t portId,
    _In_ const sai_object_list_t& sessions,
    _In_ bool is_ingress)
{
    auto port_info = m_ports.find(portId);
    if (port_info == m_ports.end()) {
        return SAI_STATUS_INVALID_PARAMETER;
    }
    
    // For simplicity, support only one session
    if (sessions.count > 1) {
        SWSS_LOG_ERROR("Multiple mirror sessions not supported");
        return SAI_STATUS_NOT_SUPPORTED;
    }
    
    if (sessions.count == 0) {
        // Disable mirroring - TODO
        return SAI_STATUS_SUCCESS;
    }
    
    sai_object_id_t session_id = sessions.list[0];
    auto session = m_mirror_sessions.find(session_id);
    if (session == m_mirror_sessions.end()) {
        return SAI_STATUS_INVALID_OBJECT_ID;
    }
    
    // Configure VPP SPAN
    vpp_mirror_t mirror;
    mirror.src_sw_if_index = port_info->second.vpp_sw_if_index;
    mirror.dst_sw_if_index = session->second.vpp_dst_sw_if_index;
    mirror.state = is_ingress ? 1 : 2;  // rx or tx
    
    int ret = span_enable_disable(&mirror, true);
    if (ret != 0) {
        SWSS_LOG_ERROR("VPP span_enable_disable failed: %d", ret);
        return SAI_STATUS_FAILURE;
    }
    
    // Update state
    session->second.vpp_src_sw_if_indices.push_back(mirror.src_sw_if_index);
    session->second.monitored_ports++;
    
    return SAI_STATUS_SUCCESS;
}
```

#### Step 6: Update Build System

**File: `platform/vpp/saivpp/src/Makefile.am`**
```makefile
# Add new source file
libsaivpp_la_SOURCES = \
    ... \
    SwitchStateBaseMirror.cpp \
    ...
```

#### Step 7: Test

**Unit Test (Optional but recommended):**
```cpp
// Create test in tests/
TEST_F(MirrorTest, CreateLocalSession)
{
    sai_attribute_t attrs[2];
    attrs[0].id = SAI_MIRROR_SESSION_ATTR_TYPE;
    attrs[0].value.s32 = SAI_MIRROR_SESSION_TYPE_LOCAL;
    attrs[1].id = SAI_MIRROR_SESSION_ATTR_MONITOR_PORT;
    attrs[1].value.oid = port_oid;
    
    sai_object_id_t session_id;
    sai_status_t status = sai->create(
        SAI_OBJECT_TYPE_MIRROR_SESSION, 
        &session_id, switch_id, 2, attrs);
    
    EXPECT_EQ(SAI_STATUS_SUCCESS, status);
}
```

**Integration Test:**
```bash
# In SONiC CLI
config mirror_session add test-span Ethernet0
config interface mirror add Ethernet4 test-span ingress

# Verify in VPP
vppctl show span
# Should show Ethernet4 → Ethernet0 mirroring

# Send test traffic through Ethernet4
# Capture on Ethernet0 to verify mirroring works
```

### 4.3 Debugging

**Common Issues and Solutions:**

**Issue 1: VPP API returns error**
```bash
# Enable VPP API tracing
vppctl api trace on
vppctl api trace save api_trace.txt
cat /tmp/api_trace.txt

# Check VPP logs
tail -f /var/log/vpp/vpp.log
```

**Issue 2: SAI attributes not mapping correctly**
```cpp
// Add debug logging
SWSS_LOG_DEBUG("attr[%d] id=%d value=%s", i, 
    attr_list[i].id, 
    sai_serialize_attr_value(...).c_str());
```

**Issue 3: Crash in syncd**
```bash
# Enable core dumps
ulimit -c unlimited

# Run syncd with debugger
gdb --args syncd -p /usr/share/sonic/hwsku/vpp.profile

# Or attach to running process
gdb -p $(pidof syncd)
```

**Issue 4: VPP not receiving commands**
```bash
# Check VPP API socket
ls -la /run/vpp/api.sock

# Verify VPP is running
ps aux | grep vpp

# Check VPP IPC connection
vppctl show api clients
```

---

## Phase 5: Contributing to sonic-vpp (Week 8)

### 5.1 Code Quality

**Follow Coding Standards:**
```bash
# C++ style (Google)
# - Use SWSS_LOG_* for logging
# - Use sai_metadata_get_* helpers
# - Check return codes
# - Add error handling

# C style (VPP xlate layer)
# - Keep it simple
# - Use VPP conventions
# - Lock before VPP API calls: VPP_LOCK() / VPP_UNLOCK()
```

**Add Documentation:**
```markdown
# Update TODO.md to mark feature as completed
# Add feature documentation in docs/HLD/
# Update CHANGELOG if applicable
```

### 5.2 Testing Requirements

**Required Tests:**

1. **Unit Tests** (at least basic coverage)
2. **VS Tests** (pytest-based tests in sonic-mgmt)
3. **Manual Testing** with real traffic

**Example pytest test:**
```python
# In sonic-mgmt/tests/vpp/
def test_mirror_session(duthost):
    """Test mirror session creation and traffic mirroring"""
    
    # Create mirror session
    duthost.shell("config mirror_session add test_span Ethernet0")
    
    # Apply to interface
    duthost.shell("config interface mirror add Ethernet4 test_span ingress")
    
    # Verify configuration
    output = duthost.shell("show mirror_session")['stdout']
    assert "test_span" in output
    
    # Send traffic through Ethernet4
    # Verify it appears on Ethernet0
    # ... PTF test logic ...
```

### 5.3 Pull Request Process

**1. Fork and Branch:**
```bash
git clone https://github.com/YOUR_USERNAME/sonic-buildimage.git
cd sonic-buildimage
git remote add upstream https://github.com/sonic-net/sonic-buildimage.git
git checkout -b feature/vpp-port-mirroring
```

**2. Commit with Good Messages:**
```bash
git commit -m "[vpp] Add port mirroring (SPAN) support

- Implement SAI_OBJECT_TYPE_MIRROR_SESSION
- Add vppxlate wrapper for sw_interface_span_enable_disable
- Support local SPAN on ports
- Add unit tests for mirror session

Tested: Manual testing with traffic, pytest tests pass
Signed-off-by: Your Name <your.email@example.com>"
```

**3. Create Pull Request:**
- Title: `[vpp] Add port mirroring (SPAN) support`
- Description: Explain what, why, how
- Link to related issues
- Add test results

**4. Address Review Comments:**
- Be responsive
- Explain your design decisions
- Make requested changes
- Re-test after changes

**5. Merge and Celebrate! 🎉**

---

## Phase 6: Advanced Topics (Ongoing)

### 6.1 Performance Optimization

**Topics to Study:**
- VPP worker thread pinning
- Batch processing of SAI operations
- Minimizing VPP API calls
- Memory management

**Tool:**
```bash
# Profile VPP performance
vppctl show runtime
vppctl show node counters
vppctl show perfmon bundle
```

### 6.2 Complex Features

**Multi-component Features:**
- QoS (requires scheduler + queues + WRED)
- Tunnels (requires encap + decap + routing)
- NAT (requires sessions + translations)

**Pattern:** Break down into smaller components

### 6.3 Upstream Contributions

**VPP Side:**
- Some features may need VPP enhancements
- Learn VPP contribution process
- Join VPP community calls

**SONiC Side:**
- Share learnings with community
- Help review other VPP PRs
- Improve documentation

---

## Resources and Community

### Documentation
- [SONiC Wiki](https://github.com/sonic-net/SONiC/wiki)
- [VPP Documentation](https://s3-docs.fd.io/vpp/25.06/)
- [SAI Specification](https://github.com/opencomputeproject/SAI)
- [sonic-platform-vpp GitHub](https://github.com/sonic-net/sonic-platform-vpp)

### Community
- SONiC Community Meetings
- VPP Community Calls
- Slack/Discord channels

### Getting Help
1. Check existing issues on GitHub
2. Ask on community channels
3. Review similar implementations
4. Consult with maintainers

---

## Sample Timeline

| Week | Focus | Deliverable |
|------|-------|-------------|
| 1-2 | SONiC & SAI fundamentals | Can explain full SONiC data flow |
| 3 | VPP basics + Integration arch | Can explain VPP Binary API |
| 4 | Dev environment + Build | Working SONiC-VPP build |
| 5-7 | Implement first feature | Working POC + tests |
| 8 | Polish + PR | Merged contribution |

---

## Success Checklist

- [ ] Built sonic-vpp from source
- [ ] Ran sonic-vpp in VM/container
- [ ] Traced a SAI API call through to VPP
- [ ] Added/modified VPP xlate function
- [ ] Implemented SAI object handler
- [ ] Wrote unit tests
- [ ] Tested with real traffic
- [ ] Created PR and got it merged
- [ ] Helped review another PR

---

## Next Steps After First Contribution

1. **Tackle more complex features**
2. **Mentor new contributors**
3. **Improve tooling and testing**
4. **Performance optimization**
5. **Documentation improvements**

Remember: Start small, iterate quickly, ask questions, and contribute incrementally!

---

**Happy Coding! Welcome to the SONiC-VPP community! 🚀**

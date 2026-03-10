# VPP Binary API Quick Reference

A practical cheatsheet for SONiC-VPP developers working with VPP Binary API.

---

## Essential Macros

### Message Allocation

```c
// Simple message (fixed size)
#define M(T, mp) \
    mp = vl_msg_api_alloc(sizeof(*mp)); \
    memset(mp, 0, sizeof(*mp)); \
    mp->_vl_msg_id = VL_API_##T; \
    mp->client_index = am->my_client_index;

// Variable-length message (arrays)
#define M2(T, extra, mp) \
    mp = vl_msg_api_alloc(sizeof(*mp) + extra); \
    // ... same as M
```

**Usage:**
```c
// Fixed size message
vl_api_sw_interface_set_flags_t *mp;
M(SW_INTERFACE_SET_FLAGS, mp);

// Variable length (e.g., N nexthops)
vl_api_ip_route_add_del_t *mp;
size_t extra = (num_nexthops - 1) * sizeof(vl_api_fib_path_t);
M2(IP_ROUTE_ADD_DEL, extra, mp);
```

### Message Send and Wait

```c
// Send message
#define S(mp) vl_msg_api_send_shmem(am->vl_input_queue, (u8 *)&mp)

// Wait for reply (blocking)
#define W(ret) \
    do { \
        ret = vl_socket_client_read(); \
        if (ret < 0) return ret; \
    } while (0);
```

**Standard Pattern:**
```c
VPP_LOCK();
M(SOME_API, mp);
mp->param1 = value1;
S(mp);
W(ret);
VPP_UNLOCK();

if (ret != 0) {
    // Handle error
}
```

---

## Common API Patterns

### 1. Simple Request-Reply

```c
int do_operation(uint32_t param)
{
    vl_api_operation_t *mp;
    int ret;
    
    VPP_LOCK();
    M(OPERATION, mp);
    mp->param = htonl(param);  // Network byte order!
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret ? -1 : 0;
}
```

### 2. Variable-Length Arrays

```c
int add_route_with_paths(int num_paths, path_info_t paths[])
{
    vl_api_ip_route_add_del_t *mp;
    size_t extra = (num_paths - 1) * sizeof(vl_api_fib_path_t);
    int ret;
    
    VPP_LOCK();
    M2(IP_ROUTE_ADD_DEL, extra, mp);
    
    mp->route.n_paths = num_paths;
    for (int i = 0; i < num_paths; i++) {
        mp->route.paths[i].sw_if_index = paths[i].sw_if_index;
        // ... fill other fields
    }
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

### 3. Dump Operations (Get Multiple Items)

```c
// Global storage for results
static interface_info_t interfaces[256];
static int interface_count = 0;

// Reply handler (called once per interface)
static void vl_api_sw_interface_details_t_handler(
    vl_api_sw_interface_details_t *mp)
{
    if (interface_count < 256) {
        interfaces[interface_count].sw_if_index = ntohl(mp->sw_if_index);
        strncpy(interfaces[interface_count].name, 
                mp->interface_name, sizeof(mp->interface_name));
        interface_count++;
    }
}

// Dump request
int get_all_interfaces(void)
{
    vl_api_sw_interface_dump_t *mp;
    vl_api_control_ping_t *mp_ping;
    int ret;
    
    interface_count = 0;
    
    VPP_LOCK();
    
    // Send dump request
    M(SW_INTERFACE_DUMP, mp);
    mp->sw_if_index = ~0;  // All interfaces
    S(mp);
    
    // Send control ping to know when done
    M(CONTROL_PING, mp_ping);
    S(mp_ping);
    W(ret);  // Wait for ping reply (all details come before this)
    
    VPP_UNLOCK();
    
    return interface_count;
}
```

### 4. Event Notifications (Async)

```c
// Enable event notifications
int enable_link_events(void)
{
    vl_api_want_interface_events_t *mp;
    int ret;
    
    VPP_LOCK();
    M(WANT_INTERFACE_EVENTS, mp);
    mp->enable_disable = 1;
    mp->pid = getpid();
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}

// Event handler (called by VPP when link state changes)
static void vl_api_sw_interface_event_t_handler(
    vl_api_sw_interface_event_t *mp)
{
    uint32_t sw_if_index = ntohl(mp->sw_if_index);
    bool link_up = mp->flags & IF_STATUS_API_FLAG_LINK_UP;
    
    // Process event (e.g., queue for SAI notification)
    handle_link_state_change(sw_if_index, link_up);
}
```

---

## Common Operations

### Interface Operations

**Set Admin State (Up/Down):**
```c
int set_interface_state(uint32_t sw_if_index, bool is_up)
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

**Add IP Address:**
```c
int add_ip_address(uint32_t sw_if_index, const char *ip, uint8_t prefix_len)
{
    vl_api_sw_interface_add_del_address_t *mp;
    ip4_address_t addr;
    int ret;
    
    // Parse IP string to binary
    ip4_address_decode(ip, &addr);
    
    VPP_LOCK();
    M(SW_INTERFACE_ADD_DEL_ADDRESS, mp);
    mp->sw_if_index = htonl(sw_if_index);
    mp->is_add = 1;
    mp->prefix.address.af = ADDRESS_IP4;
    memcpy(mp->prefix.address.un.ip4, &addr, 4);
    mp->prefix.len = prefix_len;
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

### Routing Operations

**Add/Delete Route:**
```c
int route_add_del(const char *prefix, uint8_t prefix_len,
                  const char *nexthop, bool is_add)
{
    vl_api_ip_route_add_del_t *mp;
    ip4_address_t prefix_addr, nh_addr;
    int ret;
    
    ip4_address_decode(prefix, &prefix_addr);
    ip4_address_decode(nexthop, &nh_addr);
    
    VPP_LOCK();
    M2(IP_ROUTE_ADD_DEL, sizeof(vl_api_fib_path_t), mp);
    
    mp->is_add = is_add;
    mp->route.prefix.address.af = ADDRESS_IP4;
    memcpy(mp->route.prefix.address.un.ip4, &prefix_addr, 4);
    mp->route.prefix.len = prefix_len;
    
    mp->route.n_paths = 1;
    mp->route.paths[0].sw_if_index = ~0;  // Use nexthop IP
    mp->route.paths[0].proto = FIB_API_PATH_NH_PROTO_IP4;
    memcpy(mp->route.paths[0].nh.address.ip4, &nh_addr, 4);
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

### Neighbor (ARP) Operations

**Add/Delete Neighbor:**
```c
int neighbor_add_del(uint32_t sw_if_index,
                     const uint8_t ip[4],
                     const uint8_t mac[6],
                     bool is_add)
{
    vl_api_ip_neighbor_add_del_t *mp;
    int ret;
    
    VPP_LOCK();
    M(IP_NEIGHBOR_ADD_DEL, mp);
    
    mp->is_add = is_add;
    mp->neighbor.sw_if_index = htonl(sw_if_index);
    mp->neighbor.ip_address.af = ADDRESS_IP4;
    memcpy(mp->neighbor.ip_address.un.ip4, ip, 4);
    memcpy(mp->neighbor.mac_address, mac, 6);
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

### L2 Operations

**Set Interface L2 Mode:**
```c
int set_l2_bridge(uint32_t sw_if_index, uint32_t bd_id, bool enable)
{
    vl_api_sw_interface_set_l2_bridge_t *mp;
    int ret;
    
    VPP_LOCK();
    M(SW_INTERFACE_SET_L2_BRIDGE, mp);
    
    mp->rx_sw_if_index = htonl(sw_if_index);
    mp->bd_id = htonl(bd_id);
    mp->enable = enable;
    mp->shg = 0;  // Split horizon group
    mp->port_type = L2_API_PORT_TYPE_NORMAL;
    
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    return ret;
}
```

---

## Data Type Conversions

### Network Byte Order

**Always use network byte order for multi-byte integers:**

```c
// Sending
mp->sw_if_index = htonl(sw_if_index);      // uint32_t
mp->port_id = htons(port_id);              // uint16_t

// Receiving
uint32_t sw_if_index = ntohl(mp->sw_if_index);
uint16_t port_id = ntohs(mp->port_id);
```

### IP Address Handling

**IPv4:**
```c
// String to binary
ip4_address_t addr;
inet_pton(AF_INET, "192.168.1.1", &addr);

// In message
mp->prefix.address.af = ADDRESS_IP4;
memcpy(mp->prefix.address.un.ip4, &addr, 4);
```

**IPv6:**
```c
// String to binary
ip6_address_t addr;
inet_pton(AF_INET6, "2001:db8::1", &addr);

// In message
mp->prefix.address.af = ADDRESS_IP6;
memcpy(mp->prefix.address.un.ip6, &addr, 16);
```

### MAC Address

```c
// MAC as uint8_t[6]
uint8_t mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
memcpy(mp->mac_address, mac, 6);

// Or from colon-separated string
sscanf("00:11:22:33:44:55", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
       &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
```

---

## Error Handling

### Return Codes

```c
W(ret);

// Common return codes
if (ret == 0) {
    // Success
} else if (ret == VNET_API_ERROR_NO_SUCH_ENTRY) {
    // Object doesn't exist
} else if (ret == VNET_API_ERROR_INVALID_VALUE) {
    // Invalid parameter
} else {
    // Other error
    fprintf(stderr, "VPP API error: %d\n", ret);
}
```

### Error Code Definitions

Located in `vpp/src/vnet/api_errno.h`:
```c
#define VNET_API_ERROR_UNSPECIFIED              -1
#define VNET_API_ERROR_SYSCALL_ERROR_1          -2
#define VNET_API_ERROR_SYSCALL_ERROR_2          -3
#define VNET_API_ERROR_NO_SUCH_FIB              -4
#define VNET_API_ERROR_NO_SUCH_ENTRY            -5
#define VNET_API_ERROR_INVALID_VALUE            -6
// ... more
```

---

## Debugging

### Enable API Tracing

```bash
# In VPP CLI
vppctl api trace on
vppctl api trace save /tmp/api_trace.txt

# Check trace
cat /tmp/api_trace.txt

# Filter for specific API
grep "ip_route_add_del" /tmp/api_trace.txt
```

### Common Issues

**1. Wrong Byte Order:**
```c
// WRONG
mp->sw_if_index = 5;  // Will be 5 on little-endian, wrong on wire

// RIGHT
mp->sw_if_index = htonl(5);
```

**2. Forgetting Lock:**
```c
// WRONG - Race condition!
M(SOME_API, mp);
S(mp);

// RIGHT
VPP_LOCK();
M(SOME_API, mp);
S(mp);
W(ret);
VPP_UNLOCK();
```

**3. Memory Corruption:**
```c
// WRONG - Not enough space for extra elements
M(IP_ROUTE_ADD_DEL, mp);
mp->route.n_paths = 5;  // But only allocated space for 1!

// RIGHT
size_t extra = 4 * sizeof(vl_api_fib_path_t);  // 5-1 extra
M2(IP_ROUTE_ADD_DEL, extra, mp);
mp->route.n_paths = 5;
```

**4. Not Waiting for Reply:**
```c
// WRONG - Reply gets lost
M(SOME_API, mp);
S(mp);
// No W(ret)!
// Next API call might get this reply instead

// RIGHT
S(mp);
W(ret);  // Always wait for reply
```

---

## Finding API Definitions

### Search for APIs

```bash
cd /path/to/vpp

# Find API file
find src -name "*.api" | xargs grep -l "route"

# Find specific message
grep -r "ip_route_add_del" src/vnet/

# See all messages in an API file
cat src/vnet/ip/ip.api
```

### Common API Files

| Feature | API File |
|---------|----------|
| Interfaces | `vnet/interface.api` |
| IP Routing | `vnet/ip/ip.api` |
| ARP/Neighbor | `vnet/ip-neighbor/ip_neighbor.api` |
| L2 Switching | `vnet/l2/l2.api` |
| ACL | `plugins/acl/acl.api` |
| NAT | `plugins/nat/nat44-ed/nat44_ed.api` |
| SPAN (Mirror) | `vnet/span/span.api` |
| QoS | `vnet/qos/qos.api` |

---

## SONiC-VPP Conventions

### Thread Safety

```c
// ALWAYS lock around VPP API calls
VPP_LOCK();
// ... VPP API calls ...
VPP_UNLOCK();
```

### Data Structure Convention

```c
// Interface between C++ SAI layer and C VPP layer
typedef struct _vpp_something_t {
    uint32_t field1;
    uint8_t  field2;
    // Use simple C types only
} vpp_something_t;

// Function naming
int vpp_operation_name(vpp_something_t *input, bool is_add);
```

### Error Handling Pattern

```c
int some_vpp_operation(...)
{
    vl_api_some_call_t *mp;
    int ret;
    
    VPP_LOCK();
    M(SOME_CALL, mp);
    // ... fill mp ...
    S(mp);
    W(ret);
    VPP_UNLOCK();
    
    if (ret != 0) {
        fprintf(stderr, "VPP API failed: %d\n", ret);
        return -1;
    }
    
    return 0;  // Success
}
```

---

## Quick Reference: M/M2/S/W

```c
M(TYPE, mp)           // Allocate fixed-size message
M2(TYPE, extra, mp)   // Allocate with extra bytes for arrays
S(mp)                 // Send message
W(ret)                // Wait for reply, store return code in ret
```

**Complete Example:**
```c
int example(void)
{
    vl_api_some_call_t *mp;
    int ret;
    
    VPP_LOCK();
    M(SOME_CALL, mp);           // 1. Allocate
    mp->param = htonl(value);   // 2. Fill
    S(mp);                      // 3. Send
    W(ret);                     // 4. Wait
    VPP_UNLOCK();
    
    return (ret == 0) ? 0 : -1;
}
```

---

## Resources

- **VPP API Files:** `/path/to/vpp/src/vnet/`, `/path/to/vpp/src/plugins/`
- **Generated Headers:** `vpp/build-root/install-vpp-native/vpp/include/vnet/`
- **SONiC-VPP Examples:** `platform/vpp/saivpp/src/vppxlate/SaiVppXlate.c`
- **VPP Docs:** https://s3-docs.fd.io/vpp/25.06/developer/corefeatures/api/

---

**Pro Tip:** When implementing a new feature, find a similar existing feature in `SaiVppXlate.c` and use it as a template!

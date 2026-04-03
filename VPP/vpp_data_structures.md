# VPP data structures

## hash

Header: `<vppinfra/hash.h>`

VPP's hash table is a `uword *` vector with a hidden `hash_t` header (stored *before* the vector data, same pattern as `vec_header()`). Both keys and values are `uword`-sized.

### Core types

```
hash_pair_t
├── uword  key         // integer value, or pointer cast to uword
└── uword  value[0]    // flexible array (usually 1 uword)

hash_pair_union_t       // each bucket slot is one of:
├── hash_pair_t direct          // single pair stored inline (is_user bit = 1)
└── hash_pair_indirect_t        // pointer to collision chain  (is_user bit = 0)
```

### Creation

| Macro | Key type | Key stored as | Comparison |
|-------|----------|---------------|------------|
| `hash_create(elts, value_bytes)` | integer (`uword`) | the integer itself | `==` |
| `hash_create_string(elts, value_bytes)` | C string (`char *`) | pointer to string | `strcmp` |
| `hash_create_mem(elts, key_bytes, value_bytes)` | fixed-size struct | pointer to data | `memcmp(key_bytes)` |

- `elts` — initial capacity hint (0 is fine, the table auto-grows).
- `value_bytes` — size of the value; use `sizeof(uword)` for a single `uword` value.

```c
// integer-keyed: sw_if_index → name pointer
uword *interface_name_by_sw_index = hash_create(0, sizeof(uword));

// string-keyed: interface name → sw_if_index
uword *sw_if_index_by_name = hash_create_string(0, sizeof(uword));
```

### Set (insert / update)

```c
// integer key
hash_set(h, key, value);          // h may be reassigned (auto-grow)

// pointer/string key
hash_set_mem(h, key_ptr, value);  // stores the POINTER, not a copy

// string key with automatic copy (allocates a copy of key)
hash_set_mem_alloc(&h, key_ptr, value);
```

**Important**: `hash_set_mem` stores the *pointer itself* as the key. The caller must ensure the pointed-to memory outlives the hash entry. Use `format(0, ...)` to create a heap-allocated copy:

```c
u8 *s = format(0, "%s%c", mp->interface_name, 0);   // heap copy
hash_set_mem(vam->sw_if_index_by_interface_name, s, mp->sw_if_index);
```

### Get (lookup)

`hash_get` returns `uword *` — a pointer to the value, **or `NULL` if not found**. Always check before dereferencing.

```c
// integer key
uword *p = hash_get(h, sw_if_index);
if (p) {
    u8 *name = (u8 *) p[0];
}

// string key
uword *p = hash_get_mem(h, "eth0");
if (p) {
    u32 index = (u32) p[0];
}
```

To get the full key-value pair:
```c
hash_pair_t *pair = hash_get_pair(h, key);       // integer key
hash_pair_t *pair = hash_get_pair_mem(h, key);   // pointer key
// pair->key, pair->value[0]
```

### Unset (delete)

```c
hash_unset(h, key);              // integer key — h may be reassigned
hash_unset_mem(h, key_ptr);     // pointer key

// free the key memory automatically (for hash_set_mem_alloc'd keys)
hash_unset_mem_free(&h, key_ptr);
```

### Iteration

```c
// integer keys
uword key, value;
hash_foreach(key, value, h, {
    printf("index %lu → name %p\n", key, (void *)value);
});

// pointer/string keys
u8 *name;
uword index;
hash_foreach_mem(name, index, h, {
    printf("%s → %lu\n", name, index);
});

// low-level (access hash_pair_t directly)
hash_pair_t *p;
hash_foreach_pair(p, h, {
    printf("key=%lu value=%lu\n", p->key, p->value[0]);
});
```

### Other operations

```c
hash_elts(h)         // element count (safe if h == NULL → returns 0)
hash_free(h)         // free all memory, sets h = NULL
hash_bytes(h)        // total memory usage in bytes
```

### Gotchas

1. **`hash_get` returns `uword *`, not the value.** Dereferencing a `NULL` return is a common crash.
2. **`hash_set` / `hash_unset` may reallocate** the table (auto-grow at 3/4 full). The macros assign back to `h`, but if you pass `h` into a function, pass `uword **` so the caller sees the new pointer.
3. **`hash_set_mem` stores the pointer, not a copy.** If you pass a stack buffer, the key becomes dangling after the function returns.
4. **Do not resize during `hash_foreach`** — the body must not call `hash_set` or `hash_unset` on the same table.
5. **The hash table is `uword`-based** — on 64-bit, both key and value are 8 bytes. Storing a `u32` works (it's widened), but be careful with sign extension.

### Real-world example (from `vpp_client_connect.c`)

```c
// Two hash tables for bidirectional name ↔ index lookup
uword *sw_if_index_by_interface_name;    // string → u32  (in vat_main)
uword *interface_name_by_sw_index;       // u32 → u8*     (user-defined)

// Creation
vam->sw_if_index_by_interface_name = hash_create_string(0, sizeof(uword));
interface_name_by_sw_index = hash_create(0, sizeof(uword));

// In sw_interface_details handler:
u8 *s = format(0, "%s%c", mp->interface_name, 0);
hash_set_mem(vam->sw_if_index_by_interface_name, s, mp->sw_if_index);
hash_set(interface_name_by_sw_index, mp->sw_if_index, s);

// Lookup by name:
uword *p = hash_get_mem(vam->sw_if_index_by_interface_name, "host-veth0");
if (p) printf("index = %u\n", (u32) p[0]);

// Lookup by index:
uword *p = hash_get(interface_name_by_sw_index, 3);
if (p) printf("name = %s\n", (u8 *) p[0]);
```

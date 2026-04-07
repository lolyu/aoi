# VPP data structures

## vector

Header: `<vppinfra/vec.h>`, `<vppinfra/vec_bootstrap.h>`

VPP vectors are dynamically-sized arrays similar to C++ `std::vector`, but implemented as C macros over raw pointers. A vector variable is just a typed pointer (e.g., `u32 *v`), with a hidden `vec_header_t` stored **before** the user pointer.

### Memory Layout

```
[optional heap pointer]    ← only if custom heap
[vec_header_t (8 bytes)]   ← hidden before user pointer
user_ptr → element #0
           element #1
           ...
           element #N-1
           [grow_elts spare slots]
```

### `vec_header_t` (8 bytes, hidden)

```c
typedef struct {
  u32 len;              // number of elements (not allocated capacity)
  u8 hdr_size;          // header size / VEC_MIN_ALIGN
  u8 log2_align : 7;    // data alignment (power of 2)
  u8 default_heap : 1;  // 1 = default heap, 0 = custom heap
  u8 grow_elts;         // spare slots before realloc needed
  u8 vpad[1];           // padding to 8 bytes
  u8 vector_data[0];    // user data starts here
} vec_header_t;
```

Accessed internally via `_vec_find(v)` = `((vec_header_t *)(v) - 1)`.

### Creation

```c
T *v = 0;                          // NULL is a valid empty vector
vec_new(T, N)                      // allocate N zero-initialized elements
vec_new_aligned(T, N, A)           // with alignment A
```

### Length & Capacity

```c
vec_len(v)              // element count (NULL-safe → returns 0)
vec_set_len(v, l)       // set length directly
vec_reset_length(v)     // set length to 0, keep allocation (NULL-safe)
vec_end(v)              // pointer past last element
vec_bytes(v)            // len * sizeof(element)
vec_free(v)             // free memory, set v = NULL
```

### Adding Elements

```c
vec_add1(v, elem)       // append one element (copy of elem)
vec_add2(v, ptr, N)     // append N uninitialized elements, ptr → first new slot
vec_add(v, array, N)    // append N elements copied from array
vec_resize(v, N)        // add N elements (NOT zero-initialized)
vec_validate(v, I)      // ensure v[I] exists, zero-fill any new slots
```

**`vec_validate` vs `vec_resize`:**
- `vec_validate(v, 9)` with `vec_len(v) == 3` → grows to length 10 with elements 3..9 zeroed
- `vec_resize(v, 5)` with `vec_len(v) == 3` → grows to length 8 with elements 3..7 **uninitialized**

### Removing Elements

```c
vec_pop(v)              // return and remove last element
vec_del1(v, i)          // delete v[i] by swapping with last element (O(1), unordered)
vec_delete(v, N, M)     // delete N elements starting at index M (memmove, preserves order)
```

### Insertion

```c
vec_insert(v, N, M)            // insert N zero-initialized elements at index M
vec_insert_elts(v, elts, N, M) // insert N elements from array at index M
```

### Copying

```c
vec_dup(v)              // deep copy → new allocation
vec_copy(dst, src)      // memcpy src data into dst (must be same size)
vec_clone(new_v, old_v) // allocate new_v same size as old_v (no data copy)
vec_append(v1, v2)      // append v2 to end of v1
vec_prepend(v1, v2)     // prepend v2 before v1
```

### Iteration

```c
// element pointer iteration
T *elem;
vec_foreach (elem, v) {
    // elem points to each element in order
}

vec_foreach_backwards (elem, v) {
    // reverse order
}

// index iteration
int i;
vec_foreach_index (i, v) {
    // i = 0, 1, ..., vec_len(v)-1
}
```

### Sorting & Search

```c
vec_sort_with_function(v, cmp_fn)           // qsort with comparator
vec_search(v, elem)                         // linear search, returns index or ~0
vec_search_with_function(v, elem, cmp_fn)   // search with custom comparator
```

### Other

```c
vec_zero(v)             // zero all elements
vec_is_equal(v1, v2)    // memcmp equality check
```

### Growth Strategy

When appending exceeds capacity, VPP allocates `required + required/2` (1.5× growth), giving O(1) amortized append cost. The `grow_elts` field in the header tracks spare capacity so small appends avoid reallocation.

### Gotchas

1. **`NULL` is a valid empty vector.** `vec_len(NULL)` returns 0, `vec_free(NULL)` is a no-op. Do not check `v != NULL` to test for emptiness — check `vec_len(v) == 0`.
2. **All `vec_add*` / `vec_resize` / `vec_validate` may reallocate.** Any existing pointers into the vector become dangling. The macro updates the vector variable, but not aliases.
3. **`vec_del1` does NOT preserve order.** It swaps the deleted element with the last element. Use `vec_delete` if order matters.
4. **`vec_add2` returns uninitialized memory.** The caller must fill in all fields before reading.
5. **Vectors are allocated from VPP's clib_mem heap**, not from `malloc`. The heap must be initialized with `clib_mem_init()` before any vector operation.
6. **`vec_free` sets the variable to NULL.** It's a macro, not a function — it modifies its argument.

### Real-world example (from `SaiVppXlate.c`)

```c
// Collect hash entries into a sorted vector for display
name_sort_t *nses = 0, *ns;

hash_foreach_pair (p, vam->sw_if_index_by_interface_name, ({
    vec_add2 (nses, ns, 1);        // grow by 1, ns → new slot
    ns->name = (u8 *) (p->key);
    ns->value = (u32) p->value[0];
}));

vec_sort_with_function (nses, name_sort_cmp);

vec_foreach (ns, nses) {
    print (vam->ofp, "%-25s%-15d", ns->name, ns->value);
}

vec_free (nses);    // nses = NULL after this
```

---

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

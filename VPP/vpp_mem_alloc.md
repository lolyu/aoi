# VPP Memory Mgmt

VPP does **not** use `malloc()`/`free()`. It manages its own heap using **dlmalloc** (Doug Lea's allocator) underneath a custom abstraction layer in `vppinfra/`.

---

## Initialization: `clib_mem_init`

```c
void *clib_mem_init(void *base, uword size);
```

Example: `clib_mem_init(0, 128 << 20)` — creates a 128 MB heap at a kernel-chosen address (`NULL` = auto).

Internally this:
1. Calls `clib_mem_main_init()` — detects system page size, hugepage availability, NUMA topology
2. Maps a contiguous virtual memory region via `mmap` (not `malloc`)
3. Creates a dlmalloc mspace over that region: `create_mspace_with_base(base, size, is_locked)`
4. **Disables expansion** via `mspace_disable_expand()` — the heap cannot grow beyond its initial size
5. Allocates the `clib_mem_heap_t` metadata from within the heap itself
6. Sets the heap as the current thread's active allocator

This must be the **first call** in any VPP client program because everything else — `vec_*`, `hash_*`, `format()`, `pool_*` — allocates through this heap.

Extended form:
```c
void *clib_mem_init_ex(clib_mem_init_ex_args_t *args);

typedef struct {
  void *base_addr;                      // NULL = auto-allocate
  uword memory_size;
  clib_mem_page_sz_t log2_page_sz;      // page size (log2)
  u8 alloc_free_intercept : 1;          // enable alloc/free hooks
} clib_mem_init_ex_args_t;
```

---

## Key Data Structures

### `clib_mem_heap_t` — a single heap

```c
struct clib_mem_heap_t {
  void *base;                   // base address of the mapped region
  void *mspace;                 // dlmalloc internal state
  uword size;                   // total heap size
  u8 locked : 1;                // thread-safe locking
  u8 traced : 1;                // memory tracing enabled
  u8 unmap_on_destroy : 1;
  char name[0];                 // flexible array for heap name
};
```

### `clib_mem_main_t` — global memory manager (singleton)

```c
typedef struct {
  clib_mem_heap_t *main_heap;           // primary heap
  clib_mem_heap_t **heaps;              // vector of all registered heaps
  clib_mem_thread_main_t *threads;      // linked list of thread-local data
  clib_mem_page_sz_t log2_page_sz;      // system page size
  clib_mem_page_sz_t log2_default_hugepage_sz;
  u32 numa_node_bitmap;                 // available NUMA nodes
  u8 alloc_free_intercept : 1;
} clib_mem_main_t;

extern clib_mem_main_t clib_mem_main;
```

### `clib_mem_thread_main_t` — per-thread state (thread-local)

```c
typedef struct clib_mem_thread_main_t {
  clib_mem_heap_t *active_heap;         // current thread's active heap
  clib_thread_index_t thread_index;
  struct clib_mem_thread_main_t *next;  // linked list
  int trace_thread_disable;
} clib_mem_thread_main_t;

extern __thread clib_mem_thread_main_t clib_mem_thread_main;
```

Each thread has a thread-local `active_heap` pointer, so `clib_mem_alloc()` always knows which heap to use without a parameter.

---

## Allocation API

All VPP internal allocations go through these instead of `malloc`:

| Function | Purpose |
|---|---|
| `clib_mem_alloc(size)` | Allocate from current thread's active heap |
| `clib_mem_alloc_aligned(size, align)` | Aligned allocation |
| `clib_mem_alloc_or_null(size)` | Returns NULL on failure instead of panicking |
| `clib_mem_free(p)` | Free to heap |
| `clib_mem_free_s(p)` | Secure free — zeroes memory before releasing |
| `clib_mem_realloc(p, new_size)` | Resize allocation |
| `clib_mem_heap_alloc(heap, size)` | Allocate from a specific (non-active) heap |
| `clib_mem_size(p)` | Return usable size of an allocation |

Under the hood, `clib_mem_alloc` calls `mspace_memalign(h->mspace, align, size)` on the active heap's dlmalloc mspace, with a minimum alignment of 8 bytes (`CLIB_MEM_MIN_ALIGN`).

If allocation fails and the caller used `clib_mem_alloc` (not the `_or_null` variant), VPP calls `os_out_of_memory()` and panics.

---

## Heap Switching

```c
clib_mem_heap_t *clib_mem_set_heap(clib_mem_heap_t *heap);
clib_mem_heap_t *clib_mem_get_heap(void);
```

`clib_mem_set_heap()` changes the current thread's active heap and returns the previous one. This is used when a subsystem needs to allocate from a different heap (e.g., per-numa heaps), then restore the original.

---

## Why Not malloc?

1. **Fixed size, non-growable** — predictable memory footprint, no surprise growth
2. **Hugepage support** — can map the heap on 2 MB or 1 GB pages for better TLB performance (critical for packet processing at millions of pps)
3. **NUMA awareness** — can place heaps on specific NUMA nodes to avoid cross-socket memory access
4. **No fragmentation from mixed allocators** — VPP's data structures and system libraries don't share the same allocator
5. **Built-in tracing** — `clib_mem_trace_enable()` records all allocations with stack traces (like a built-in Valgrind)
6. **ASAN integration** — `clib_mem_unpoison()` / `clib_mem_poison()` calls at every alloc/free for sanitizer support

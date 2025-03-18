#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>

/*
malloc version:
1000000 nodes creation took 27160028 ns
simple_alloc:
1000000 nodes creation took 11860307 ns
posix_memalign:
1000000 nodes creation took 4142446 ns
node with single byte padding:
1000000 nodes creation took 4518632 ns
with prefetch:
1000000 nodes creation took 2787854 ns
*/

typedef struct node  {
    char buffer[24];
    struct node *next;
} node_t;

// typedef struct __attribute__((packed)) node {
//     char pad;
//     struct node *next;
//     char pad2;
//     char buffer[24];
// } node_t;

#define NODENUM                     1000000
#define NODES                       1000000
#define SIMPLE_ALLOCATOR_SIZE       100000000UL
const char PAYLOAD[] = "This is a text";
static void *start_ = NULL;
static void *cur_ = NULL;
static void *end_ = NULL;
static size_t counter_ = 0;

void init_alloc() {
    if (start_ == NULL) {
        // start_ = malloc(SIMPLE_ALLOCATOR_SIZE);
        // if (start_ == NULL) {
        //     std::cerr << "Error allocating " << SIMPLE_ALLOCATOR_SIZE << " bytes for allocator!" << std::endl;
        //     exit(1);
        // }
        int rc = posix_memalign(&start_, 64, SIMPLE_ALLOCATOR_SIZE);
        if (rc != 0)
        {
            std::cerr << "Error allocating " << SIMPLE_ALLOCATOR_SIZE << " bytes for allocator!" << std::endl;
            exit(1);
        }
        __builtin_prefetch(start_ + 16 * 64);
        memset(start_, 0, SIMPLE_ALLOCATOR_SIZE);
        cur_ = start_;
        end_ = start_ + SIMPLE_ALLOCATOR_SIZE;
        std::cout << "start_: " << start_ << ", end: " << end_ << std::endl;
    }
}

static void* simple_alloc(size_t size) {
    if (cur_ + size <= end_) {
      void *ptr = cur_;
      cur_ += size;
      counter_++;
      __builtin_prefetch(cur_ + 16 * 64);
      return ptr;
    } else {
       std::cerr << "Error allocating " << size << " bytes!" << std::endl;
    }
    return NULL;
}

node_t *new_node(node_t *prev, const char *payload, size_t size)
{
    node_t *n = (node_t *)simple_alloc(sizeof(node_t));
    if (n == NULL)
    {
        std::cerr << "Error allocating " << sizeof(node_t) << " bytes for struct node!" << std::endl;
        exit(1);
    }

    memcpy(n->buffer, payload, size);
    n->next = NULL;

    if (prev)
    {
        prev->next = n;
    }

    return n;
}

node_t *alloc_nodes()
{
    auto start = std::chrono::high_resolution_clock::now();
    node_t *head = new_node(NULL, PAYLOAD, strlen(PAYLOAD));
    node_t *node = head;
    for (int i = 0; i < NODENUM; ++i)
    {
        node = new_node(node, PAYLOAD, strlen(PAYLOAD));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << NODENUM << " nodes creation took " << duration.count() << " ns" << std::endl;
    return head;
}

void free_nodes(node_t *n)
{
    if (!n) return;
    while (n->next)
    {
        node_t *next = n->next;
        free(n);
        n = next;
    }
}

int main()
{
    std::cout << sizeof(node_t) << std::endl;
    init_alloc();
    node_t *node = alloc_nodes();
    free(start_);
    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <assert.h>

/**
 * Basic requirements:
 * 1. tracking block metadata(allocated/mfree)
 * 2. searching for and managing mfree space
 * 3. performing allocations
 * 4. singly-linked list
 *
 * NOTE: Linux requires 8-bytes alignment
 */

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

void *mmalloc(size_t size);
void mfree(void *ptr);
void *mrealloc(void *ptr, size_t size);
void test_implementation();
void walk_heap();

void *break_start = NULL;
void *break_end = NULL;

int main()
{
    test_implementation();
    return 0;
}

void walk_heap()
{
    assert(break_start);

    void *p;
    size_t header, blk_size;
    int allocated;

    for (p = break_start; p < break_end; p = (char *)p + blk_size)
    {
        header = *(size_t *)p;
        blk_size = header & ~0x1;
        allocated = header & 0x1;
        printf("block size %ld, allocated %d.\n", blk_size, allocated);
    }
}

void *find_fit(size_t size)
{
    size_t *header = (size_t *)break_start;
    size_t *next;
    while ((void *)header < break_end)
    {
        if (!(*header & 0x1))
        {
            if (*header >= size)
            {
                return header;
            }

            // defered coalescing
            // indeterminate performance during allocation
            // merge next block if it is available && free
            next = (size_t *)((char *)header + *header);
            if ((void *)next < break_end && !(*next & 0x1))
            {
                *header += *next;
                continue;
            }
        }
        header = (size_t *)((char *)header + (*header & ~0x1));
    }
    return NULL;
}

void *mmalloc(size_t size)
{
    if (!break_start)
        break_start = sbrk(0);

    // blk_size is the header size + payload size
    size_t blk_size = ALIGN(size + SIZE_T_SIZE);
    void *header = (size_t *)find_fit(blk_size);
    if (header)
    {
        if (*(size_t *)header > blk_size)
        {
            *(size_t *)((char *)header + blk_size) = *(size_t *)header - blk_size;
            *(size_t *)header = blk_size;
        }
        *(size_t *)header |= 0x1;
    }
    else
    {
        header = sbrk(blk_size);
        *(size_t *)header = blk_size | 0x1;
        break_end = (char *)header + blk_size;
    }

    return (char *)header + SIZE_T_SIZE;
}

void mfree(void *ptr)
{
    void *header = (char *)ptr - SIZE_T_SIZE;
    *(size_t *)header &= ~(0x1);
}

void *mrealloc(void *ptr, size_t size)
{
}

void test_implementation()
{
    void *ptr0 = mmalloc(10); // -> 16 + 8 = 24
    void *ptr1 = mmalloc(20); // -> 24 + 8 = 32
    void *ptr2 = mmalloc(30); // -> 32 + 8 = 40
    void *ptr4 = mmalloc(40); // -> 40 + 8 = 48

    mfree(ptr1);
    mfree(ptr2);

    void *ptr3 = mmalloc(40); // -> 40 + 8 = 48
    void *ptr5 = mmalloc(40); // -> 40 + 8 = 48

    walk_heap();
    mfree(ptr0);
    mfree(ptr3);
    mfree(ptr4);
    mfree(ptr5);
}

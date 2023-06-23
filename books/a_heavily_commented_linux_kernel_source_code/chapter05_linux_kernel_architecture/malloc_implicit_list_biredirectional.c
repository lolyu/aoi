#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Basic requirements:
 * 1. tracking block metadata(allocated/mfree)
 * 2. searching for and managing mfree space
 * 3. performing allocations
 * 4. bidirectional singly-linked list
 *
 * NOTE: Linux requires 8-bytes alignment
 *
 * In summary:
 * malloc(): O(N) N = block count
 * free(): O(1)
 * realloc(): O(M + N) N = block count, M = block size
 */

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define MIN_BLOCK_SIZE ALIGN(SIZE_T_SIZE + SIZE_T_SIZE + 1)
#define IS_BLOCK_ALLOCATED(ptr) ((short)(*(HEADER((ptr))) & 0x1))
#define ALLOCATE_BLOCK(ptr, blk_size)        \
    do                                       \
    {                                        \
        *(HEADER((ptr))) = (blk_size | 0x1); \
        *(FOOTER((ptr))) = (blk_size | 0x1); \
    } while (0)

#define DEALLOCATE_BLOCK(ptr)       \
    do                              \
    {                               \
        (*(HEADER((ptr))) &= ~0x1); \
        (*(FOOTER((ptr))) &= ~0x1); \
    } while (0)

#define BLOCK_SIZE(ptr) (*(HEADER((ptr))) & ~0x1)
#define NEXT_BLOCK(ptr) ((void *)((char *)(ptr) + (*((size_t *)(ptr)) & ~0x1)))
#define PREV_BLOCK(ptr) ((void *)((char *)(ptr) - (*(size_t *)(((char *)(ptr)) - SIZE_T_SIZE) & ~0x1)))
#define HEADER(ptr) ((size_t *)(ptr))
#define FOOTER(ptr) ((size_t *)((char *)(ptr) + (*((size_t *)(ptr)) & ~0x1) - SIZE_T_SIZE))
#define HEADER_FROM_BLK_P(ptr) ((size_t *)(((char *)(ptr)) - SIZE_T_SIZE))
#define PREV_BLOCK_HEADER(ptr) HEADER(PREV_BLOCK((ptr)))
#define PREV_BLOCK_FOOTER(ptr) ((size_t *)(((char *)(ptr)) - SIZE_T_SIZE))
#define NEXT_BLOCK_HEADER(ptr) HEADER(NEXT_BLOCK((ptr)))
#define NEXT_BLOCK_FOOTER(ptr) FOOTER(NEXT_BLOCK((ptr)))

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
    size_t *header, *footer;

    for (p = break_start; p < break_end; p = NEXT_BLOCK(p))
    {
        header = HEADER(p);
        footer = FOOTER(p);
        assert(*header == *footer);
        printf("block size %ld, allocated %d.\n", BLOCK_SIZE(p), IS_BLOCK_ALLOCATED(p));
    }
}

void *find_fit(size_t size)
{
    /**
     * three search heuristics:
     * 1. first-fit
     * 2. next-fit
     * 3. best-fit
     */
    size_t *header = (size_t *)break_start;
    size_t *next;
    while ((void *)header < break_end)
    {
        next = (size_t *)NEXT_BLOCK(header);
        if (!IS_BLOCK_ALLOCATED(header))
        {
            if (*header >= size)
            {
                return header;
            }

            // defered coalescing
            // indeterminate performance during allocation
            // merge next block if it is available && free
            if ((void *)next < break_end && !IS_BLOCK_ALLOCATED(next))
            {
                *header += BLOCK_SIZE(next);
                *FOOTER(header) = *header;
                continue;
            }
        }
        header = next;
    }
    return NULL;
}

void *mmalloc(size_t size)
{
    if (!break_start)
    {
        void *prologue = sbrk(ALIGN(SIZE_T_SIZE + SIZE_T_SIZE));
        ALLOCATE_BLOCK(prologue, ALIGN(SIZE_T_SIZE + SIZE_T_SIZE));
        break_start = NEXT_BLOCK(prologue);
    }

    // blk_size is the header size + payload size
    size_t blk_size = ALIGN(size + SIZE_T_SIZE + SIZE_T_SIZE);
    void *header = (size_t *)find_fit(blk_size);
    if (header)
    {
        // split the large block, but check if the split next block is
        // large enough to hold a min block.
        size_t allocated_size = BLOCK_SIZE(header);
        if (allocated_size >= blk_size + MIN_BLOCK_SIZE)
        {
            // set current block header
            *HEADER(header) = blk_size;
            // set next block header
            *NEXT_BLOCK_HEADER(header) = allocated_size - blk_size;
            // set next block footer
            *NEXT_BLOCK_FOOTER(header) = allocated_size - blk_size;
        }
        blk_size = BLOCK_SIZE(header);
        ALLOCATE_BLOCK(header, blk_size);
    }
    else
    {
        header = sbrk(blk_size);
        ALLOCATE_BLOCK(header, blk_size);
        break_end = (char *)header + blk_size;
    }

    return (char *)header + SIZE_T_SIZE;
}

void mfree(void *ptr)
{
    // immediately coalescing
    size_t *header = HEADER_FROM_BLK_P(ptr);
    size_t blk_size = BLOCK_SIZE(header);

    size_t *prev = PREV_BLOCK_HEADER(header);
    size_t *next = NEXT_BLOCK_HEADER(header);
    int prev_allocated = (void *)prev < break_start || IS_BLOCK_ALLOCATED(prev);
    int next_allocated = (void *)next >= break_end || IS_BLOCK_ALLOCATED(next);

    if (prev_allocated && next_allocated)
    {
        DEALLOCATE_BLOCK(header);
    }
    else if (prev_allocated && !next_allocated)
    {
        DEALLOCATE_BLOCK(header);
        blk_size += (BLOCK_SIZE(next));
        *HEADER(header) = *FOOTER(next) = blk_size;
    }
    else if (!prev_allocated && next_allocated)
    {
        blk_size += (BLOCK_SIZE(prev));
        *HEADER(prev) = *FOOTER(header) = blk_size;
    }
    else
    {
        blk_size += (BLOCK_SIZE(prev) + BLOCK_SIZE(next));
        *HEADER(prev) = *FOOTER(next) = blk_size;
    }
}

void *mrealloc(void *ptr, size_t size)
{
    size_t *header = HEADER_FROM_BLK_P(ptr);
    size_t old_size = BLOCK_SIZE(ptr);
    size_t new_size = ALIGN(size + SIZE_T_SIZE + SIZE_T_SIZE);
    void *newptr;
    if (old_size >= new_size)
    {
        return ptr;
    }
    else
    {
        mfree(ptr);
        newptr = mmalloc(new_size);
        if (newptr != ptr)
        {
            memcpy(newptr, ptr, old_size - SIZE_T_SIZE - SIZE_T_SIZE);
        }
        return newptr;
    }
}

void test_implementation()
{
    void *ptr0 = mmalloc(10); // 10 + 8 + 8 = 26 -> 32 -> 0x010 ~ 0x030
    void *ptr1 = mmalloc(20); // 20 + 8 + 8 = 36 -> 40 -> 0x030 ~ 0x058
    void *ptr2 = mmalloc(30); // 30 + 8 + 8 = 46 -> 48 -> 0x058 ~ 0x088
    void *ptr3 = mmalloc(40); // 40 + 8 + 8 = 56 -> 56 -> 0x088 ~ 0x0c0
    void *ptr4 = mmalloc(50); // 50 + 8 + 8 = 66 -> 72 -> 0x0c0 ~ 0x118
    void *ptr5 = mmalloc(60); // 60 + 8 + 8 = 76 -> 80 -> 0x118 ~ 0x168

    mfree(ptr2);
    mfree(ptr3);

    mfree(ptr0);
    mfree(ptr1);
    mfree(ptr4);
    mfree(ptr5);

    void *ptr6 = mmalloc(15); // 15 + 8 + 8 = 31 -> 32
    walk_heap();
}

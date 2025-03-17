# cache prefetch
* cache prefetch could improve the system performance due to the locality:
    * temporal locality: if a particular memory location is referenced, it is likely that the same location will be reference again.
    * spatial locality (memory locality): if a particular memory location is referenced, it is likely that nearby memory locations will be referenced again.

## gcc `__builtin_prefetch`
```c++
#include <chrono>
#include <iostream>
#include <mmintrin.h>

#define NROWS 512
#define NCOLS 512
#define CACHE_LINE_SIZE 64

int array[NROWS][NCOLS];


void init_array_prefetch()
{
    int i, j, jj;
    int stride = CACHE_LINE_SIZE / sizeof(int);
    for (i = 0; i < NROWS; ++i)
    {
        for (j = 0; j < NCOLS; j += stride)
        {
            __builtin_prefetch(&array[i][j], 0, 0);
            for (jj = j; jj < j + stride; ++jj)
            {
                array[i][jj] = 100;
            }
        }
    }
}

void init_array()
{
    int i, j;
    for (i = 0; i < NROWS; ++i)
    {
        for (j = 0; j < NCOLS; ++j)
        {
            array[i][j] = 100;
        }
    }
}

void time_func_call(void (*func)())
{
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Function execution time: " << duration.count() << " microseconds" << std::endl;
}

int main()
{
    time_func_call(init_array);
    time_func_call(init_array_prefetch);
    return 0;
}

```
* the prefetch could decrease the init function runtime from 950ms to 780ms.
```
$ ./a.out         
Function execution time: 949 microseconds
$ ./a.out         
Function execution time: 774 microseconds
```

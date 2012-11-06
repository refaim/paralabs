#include <omp.h>
#include <string.h>
#include "mergesort.h"
#include "quicksort.h"

#define PERFORMANCE_BARRIER 20

void merge(void *a, void *b, uint half, uint rest, size_t width,
           comparator_t comparator)
{
    void *left = malloc(half * width), *right = malloc(rest * width);

    memcpy(left, a, half * width);
    memcpy(right, b, rest * width);

    for (uint i = 0, j = 0, k = 0; k < half + rest; ++k)
    {
        if (j == rest || (i < half &&
            comparator(at(left, width, i), at(right, width, j)) <= 0))
        {
            memcpy(at(a, width, k), at(left, width, i++), width);
        }
        else
        {
            memcpy(at(a, width, k), at(right, width, j++), width);
        }
    }

    free(left);
    free(right);
}

void mergesort(void *base, uint num, size_t width, comparator_t comparator)
{
    if (num < PERFORMANCE_BARRIER)
    {
        quicksort(base, num, width, comparator);
    }
    else if (num > 1)
    {
        uint half = num / 2;
        uint rest = num - half;
        void *a = base;
        void *b = at(base, width, half);

        #pragma omp parallel sections
        {
            #pragma omp section
            mergesort(a, half, width, comparator);
            #pragma omp section
            mergesort(b, rest, width, comparator);
        }

        merge(a, b, half, rest, width, comparator);
    }
}

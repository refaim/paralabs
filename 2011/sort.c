#include <omp.h>
#include "sort.h"

void *at(char *base, size_t width, uint shift)
{
    return base + shift * width;
}

void swap(char *a, char *b, size_t width)
{
    char tmp;

    if (a != b)
    {
        while (width--)
        {
            tmp = *a;
            *a++ = *b;
            *b++ = tmp;
        }
    }
}

int cmpint(const void *n, const void *m)
{
    return *(const int *)n - *(const int *)m;
}

int cmpdouble(const void* n, const void* m)
{
    double diff = *(const double *)n - *(const double *)m;

    if (fabs(diff) < EPS) return 0;
    return diff > 0 ? 1 : -1;
}

int check_order(void *base, uint num, size_t width, comparator_t comparator)
{
    for (uint i = 1; i < num; ++i)
    {
        if (comparator(at(base, width, i - 1), at(base, width, i)) > 0)
        {
            return 1;
        }
    }
    return 0;
}

void insertionsort(void *base, uint num, size_t width, comparator_t comparator)
{
    uint i, j;

    #pragma omp parallel for private(j)
    for (i = 1; i < num; ++i)
    {
        j = i;
        while (j > 0 &&
            comparator(at(base, width, j - 1), at(base, width, j)) > 0)
        {
            swap(at(base, width, j), at(base, width, j - 1), width);
            j--;
        }
    }
}

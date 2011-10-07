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

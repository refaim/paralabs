#include "quicksort.h"

int partition(void *base, uint num, size_t width, comparator_t comparator)
{
    void *fixed = at(base, width, num - 1);
    void *current;
    uint i = 0;

    for (uint j = 0; j < num - 1; ++j)
    {
        current = at(base, width, j);
        if (comparator(current, fixed) <= 0)
        {
            swap(at(base, width, i++), current, width);
        }
    }
    swap(at(base, width, i), fixed, width);
    return i;
}

void quicksort(void* base, uint num, size_t width, comparator_t comparator)
{
    if (num > 0)
    {
        int pivot = partition(base, num, width, comparator);
        quicksort(base, pivot, width, comparator);
        quicksort(
            at(base, width, pivot + 1),
            num - pivot - 1, width, comparator);
    }
}

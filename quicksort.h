#ifndef _QUICKSORT_H_
#define _QUICKSORT_H_

#include "sort.h"

int partition(void *base, uint num, size_t width, comparator_t comparator);
void quicksort(void* base, uint num, size_t width, comparator_t comparator);

#endif

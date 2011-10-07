#ifndef _SORT_H_
#define _SORT_H_

#include <math.h>

#include "common.h"

#define EPS 10e-10

typedef int (*comparator_t) (const void *, const void *);

void *at(char *base, size_t width, uint shift);
void swap(char *a, char *b, size_t width);

int cmpint(const void *n, const void *m);
int cmpdouble(const void* n, const void* m);

#endif

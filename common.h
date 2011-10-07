#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define uint unsigned int

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#define ALLOC_CHECK(mem) check_alloc(mem, __FILE__, __LINE__)
void *check_alloc(void *memory, char* file, int line);

double diff_to_sec(struct timeval *a, struct timeval *b);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void *check_alloc(void *memory, char* file, int line)
{
    if (memory == NULL)
    {
        fprintf(stderr, "allocation error on %s:%d", file, line);
        exit(EXIT_FAILURE);
    }
    return memory;
}

double diff_to_sec(struct timeval *a, struct timeval *b)
{
    return b->tv_sec - a->tv_sec + 10e-7 * (b->tv_usec - a->tv_usec);
}

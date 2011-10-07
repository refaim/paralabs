#include <stdlib.h>
#include <sys/time.h>
#include "common.h"

void generate_int_sequence(int **dest, uint count)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);

    ALLOC_CHECK(*dest = malloc(count * sizeof(int)));

    for (uint i = 0; i < count; ++i)
    {
        (*dest)[i] = rand();
    }
}

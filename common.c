#include <sys/time.h>

double diff_to_sec(struct timeval *a, struct timeval *b)
{
    return b->tv_sec - a->tv_sec + 10e-7 * (b->tv_usec - a->tv_usec);
}

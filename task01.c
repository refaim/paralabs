#include <stdio.h>
#define __USE_BSD
#include <math.h>

#include "pcheck.h"
#include "common.h"

#define MAXN 10000000
#define CALC_VALUE 10000.0f


void* first_body(void* UNUSED(arg))
{
    double root;
    double value = CALC_VALUE;
    int i;

    printf("first: start\n");
    for (i = 0; i < MAXN; ++i)
    {
        root = (value * value) / M_PI * M_E;
    }
    printf("%d iterations\n", i);
    printf("first: stop\n");

    return NULL;
}

void* second_body(void *arg)
{
    pthread_t* blocker = (pthread_t*)arg;

    PCheck(pthread_join(*blocker, NULL));
    printf("second\n");

    return NULL;
}

int main()
{
    pthread_t first, second;

    PCheck(pthread_create(&first, NULL, first_body, NULL));
    PCheck(pthread_create(&second, NULL, second_body, &first));
    PCheck(pthread_join(second, NULL));

    return EXIT_SUCCESS;
}

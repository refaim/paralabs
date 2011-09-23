#include <stdio.h>
#include <pthread.h>

#define __USE_BSD
#include <math.h>

#include <pp.h>

#define MAXN 10000000
#define CALC_VALUE 10000.0f


void* first_body(void* arg)
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
}

void* second_body(void *arg)
{
    pthread_t* blocker = (pthread_t*)arg;

    PTCALLEX(pthread_join(*blocker, NULL), NULL)
    printf("second\n");
}

int main()
{
    pthread_t first, second;

    PTCALL(pthread_create(&first, NULL, first_body, NULL))
    PTCALL(pthread_create(&second, NULL, second_body, &first))
    PTCALL(pthread_join(second, NULL))
}

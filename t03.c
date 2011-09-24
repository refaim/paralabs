#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <pp.h>

#define N 120

int gCounter = 0;

void* first_body(void* arg)
{
    for (int i = 2; i <= N; i += 2)
    {
        while (gCounter % 2 == 0)
        {
            sleep(1);
        }
        printf("%d\n", i);
        gCounter = i;
    }
}

void* second_body(void *arg)
{
    for (int i = 1; i <= N + 1; i += 2)
    {
        while (gCounter % 2 != 0)
        {
            sleep(1);
        }
        printf("%d\n", i);
        gCounter = i;
    }
}

int main()
{
    pthread_t first, second;

    PTCALL(pthread_create(&first, NULL, first_body, NULL))
    PTCALL(pthread_create(&second, NULL, second_body, NULL))
    PTCALL(pthread_join(first, NULL))
    PTCALL(pthread_join(second, NULL))
}

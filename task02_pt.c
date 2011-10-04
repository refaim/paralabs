#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include <pp.h>

#define N 120

int gCounter = 0;

void* first_body(void* UNUSED(arg))
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
    return NULL;
}

void* second_body(void* UNUSED(arg))
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
    return NULL;
}

int main()
{
    pthread_t first, second;

    PCheck(pthread_create(&first, NULL, first_body, NULL));
    PCheck(pthread_create(&second, NULL, second_body, NULL));
    PCheck(pthread_join(first, NULL));
    PCheck(pthread_join(second, NULL));

    return EXIT_SUCCESS;
}

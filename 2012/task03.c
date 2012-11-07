#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100
static int numbers[N];

void printer(int from, int* to, int step)
{
    for (int i = from; i < *to; i += step)
    {
        numbers[i] = i + 1;
    }
}

void* first_body(void* arg)
{
    printer(0, arg, 2);
    return NULL;
}

void* second_body(void* arg)
{
    printer(1, arg, 2);
    return NULL;
}

int main()
{
    pthread_t first_thread, second_thread;
    int count = N;

    pthread_create(&first_thread, NULL, first_body, &count);
    pthread_create(&second_thread, NULL, second_body, &count);

    pthread_join(first_thread, NULL);
    pthread_join(second_thread, NULL);

    for (int i = 0; i < N; ++i)
    {
        printf("%d\n", numbers[i]);
    }

    return EXIT_SUCCESS;
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t event;
} sync_t;

void printer(sync_t* sync, int from, int to, int step)
{
    pthread_mutex_lock(&sync->mutex);
    for (int i = from; i < to; i += step)
    {
        printf("%d\n", i);
        pthread_cond_signal(&sync->event);
        pthread_cond_wait(&sync->event, &sync->mutex);
    }
    pthread_cond_signal(&sync->event);
    pthread_mutex_unlock(&sync->mutex);
}

void* print_even(void* arg)
{
    printer(arg, 2, N + 1, 2);
    return NULL;
}

void* print_odd(void* arg)
{
    printer(arg, 1, N, 2);
    return NULL;
}

int main()
{
    pthread_t even_thread, odd_thread;
    sync_t sync;

    pthread_mutex_init(&sync.mutex, NULL);
    pthread_cond_init(&sync.event, NULL);

    pthread_create(&even_thread, NULL, print_even, &sync);
    pthread_create(&odd_thread, NULL, print_odd, &sync);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    return EXIT_SUCCESS;
}

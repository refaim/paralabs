#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "bank/event.h"

#define N 100

typedef struct
{
    event_t odd;
    event_t even;
} events_t;

void* print_even(void* arg)
{
    events_t* events = arg;
    for (int i = 2; i < N + 1; i += 2)
    {
        printf("%d\n", i);
        event_send(&events->even);
        event_wait(&events->odd);
    }
    event_send(&events->even);
    return NULL;
}

void* print_odd(void* arg)
{
    events_t* events = arg;
    for (int i = 1; i < N; i += 2)
    {
        printf("%d\n", i);
        event_send(&events->odd);
        event_wait(&events->even);
    }
    event_send(&events->odd);
    return NULL;
}

int main()
{
    pthread_t even_thread, odd_thread;
    events_t events;

    event_init(&events.odd);
    event_init(&events.even);

    pthread_create(&odd_thread, NULL, print_odd, &events);
    event_wait(&events.odd);
    pthread_create(&even_thread, NULL, print_even, &events);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    return EXIT_SUCCESS;
}

#include "event.h"

void event_init(event_t* event)
{
    pthread_cond_init(&event->cond, NULL);
    pthread_mutex_init(&event->mutex, NULL);
    event->happened = 0;
}

void event_wait(event_t* event)
{
    pthread_mutex_lock(&event->mutex);
    while (!event->happened)
    {
        pthread_cond_wait(&event->cond, &event->mutex);
    }
    event->happened = 0;
    pthread_mutex_unlock(&event->mutex);
}

void event_send(event_t* event)
{
    pthread_mutex_lock(&event->mutex);
    if (!event->happened)
    {
        pthread_cond_signal(&event->cond);
        event->happened = 1;
    }
    pthread_mutex_unlock(&event->mutex);
}

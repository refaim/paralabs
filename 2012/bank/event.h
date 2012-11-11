#ifndef __EVENT_H__
#define __EVENT_H__

#include <pthread.h>

typedef struct
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int happened;
} event_t;

void event_init(event_t* event);
void event_wait(event_t* event);
void event_send(event_t* event);

#endif // __EVENT_H__

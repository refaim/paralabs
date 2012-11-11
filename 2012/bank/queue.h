#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <pthread.h>

typedef struct node
{
    struct node* next;
    void* message;
} node_t;

typedef struct
{
    node_t* head;
    node_t* tail;
    pthread_mutex_t lock;
} queue_t;

void push(queue_t* queue, void* message);
void* pop(queue_t* queue);

#endif // __QUEUE_H__

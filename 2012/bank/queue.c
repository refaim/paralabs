#include <stdlib.h>

#include "queue.h"

void push(queue_t* queue, void* message)
{
    pthread_mutex_lock(&queue->lock);

    node_t* node = malloc(sizeof(node_t));
    node->message = message;
    node->next = NULL;

    if (queue->tail == NULL)
    {
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
    }

    if (queue->head == NULL)
    {
        queue->head = node;
    }

    pthread_mutex_unlock(&queue->lock);
}

void* pop(queue_t* queue)
{
    void* message;
    node_t* target;

    pthread_mutex_lock(&queue->lock);

    if (queue->head == NULL)
    {
        return NULL;
    }

    message = queue->head->message;

    target = queue->head;
    queue->head = queue->head->next;
    free(target);

    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }

    pthread_mutex_unlock(&queue->lock);

    return message;
}

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define EMPTY_DATA ((void*)0xCAFEBAB5)

/**
 * Create a new Queue struct
 * @return new Queue struct pointer
 */
Queue* Queue_init()
{
    Queue* this = malloc(sizeof(Queue));
    pthread_mutex_init(&this->head_lock, NULL);
    pthread_mutex_init(&this->tail_lock, NULL);

    this->divider.next = NULL;
    this->head = &this->divider;
    this->tail = &this->divider;
    this->size = 0;
    return this;
}

//void INIT_QUEUE_HEAD(QueueNode* node)
//{
//    node->next = EMPTY_DATA;
//}

/**
 * Put node to queue
 * @param node node struct pointer to put into queue
 * @param queue queue struct pointer
 */
void Queue_put(QueueNode* node, Queue* queue)
{
    node->next = NULL;

    //atomic push to queue by acquiring mutex lock
    pthread_mutex_lock(&queue->tail_lock);
    queue->tail->next = node;
    queue->tail = node;
    queue->size = queue->size + 1;
    pthread_mutex_unlock(&queue->tail_lock);
}

/**
 * Get first element from queue
 * @param queue queue struct pointer
 * @return first node struct pointer, NULL if empty
 */
QueueNode* Queue_get(Queue* queue)
{
    QueueNode *head, *next;

    while (1) {
        pthread_mutex_lock(&queue->head_lock);
        head = queue->head;
        next = head->next;
        if (next == NULL) {
            pthread_mutex_unlock(&queue->head_lock);
            return NULL;
        }
        queue->head = next;

        if (queue->size > 0) queue->size = queue->size - 1;
        pthread_mutex_unlock(&queue->head_lock);

        if (head == &queue->divider) {
            Queue_put(head, queue);
            continue;
        }

        head->next = EMPTY_DATA;
        return head;
    }
}

/**
 * Look at first element from queue but don't dequeue
 * @param queue queue struct pointer
 * @return pointer to data of first node in the queue
 */
void* Queue_peek(Queue *queue) {
    pthread_mutex_lock(&queue->head_lock);
    void* data = queue->head->data;
    if (queue->head == &queue->divider) {
        if (queue->head->next != NULL) {
            data = queue->head->next->data;
            pthread_mutex_unlock(&queue->head_lock);
            return data;
        }
        else {
            pthread_mutex_unlock(&queue->head_lock);
            return NULL;
        }
    }

    pthread_mutex_unlock(&queue->head_lock);
    return data;
}

/**
 * Print queue info
 * @param queue queue struct pointer
 */
void Queue_print(Queue *queue) {
    printf("Queue size: %d\n", queue->size);
    QueueNode* node = queue->head;
    while (node != NULL) {
        if (node == &queue->divider){
            node = node->next;
            continue;
        }
        printf("Node data %d\n", *((int *) node->data));
        node = node->next;
    }
    printf("End reached\n");
}
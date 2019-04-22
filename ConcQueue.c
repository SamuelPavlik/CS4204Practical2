#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

#define EMPTY_NODE ((void*)0xCAFEBAB5)

/**
 * Create a new Queue struct
 * @return new Queue struct pointer
 */
Queue* Queue_init()
{
//    Queue* this = malloc(sizeof(Queue));
//    this->headLock = malloc(sizeof(pthread_t));
//    this->tailLock = malloc(sizeof(pthread_t));
//    pthread_mutex_init(this->headLock, NULL);
//    pthread_mutex_init(this->tailLock, NULL);
//
////    long unsigned int x = sizeof(QueueNode);
//    this->divider = malloc(sizeof(QueueNode));
//    this->divider->next = NULL;
//    this->divider->data = NULL;
//    this->head = this->divider;
//    this->tail = this->divider;
//    this->size = 0;
//    return this;

    Queue* this = malloc(sizeof(Queue));
    pthread_mutex_init(&this->headLock, NULL);
    pthread_mutex_init(&this->tailLock, NULL);

    this->divider.next = NULL;
    this->divider.data = NULL;
    this->head = &this->divider;
    this->tail = &this->divider;
    this->size = 0;
    return this;
}

/**
 * destroy this queue
 * @param this queue struct pointer
 */
void Queue_destroy(Queue* this) {
    free(this);
}

/**
 * Put node to queue
 * @param node node struct pointer to put into queue
 * @param queue queue struct pointer
 */
void Queue_put(QueueNode* node, Queue* queue)
{
    node->next = NULL;

    //atomic push to queue by acquiring mutex lock
    pthread_mutex_lock(&queue->tailLock);
    queue->tail->next = node;
    queue->tail = node;
    queue->size = queue->size + 1;
    pthread_mutex_unlock(&queue->tailLock);
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
        pthread_mutex_lock(&queue->headLock);
        head = queue->head;
        next = head->next;
        //No next element means that this element is the divider element and therefore the queue is empty
        if (next == NULL) {
            pthread_mutex_unlock(&queue->headLock);
            return NULL;
        }
        queue->head = next;

        if (queue->size > 0) queue->size = queue->size - 1;
        pthread_mutex_unlock(&queue->headLock);

        //If divider, then enqueue back and retrieve next element
        if (head == &queue->divider) {
            Queue_put(head, queue);
            continue;
        }

        head->next = EMPTY_NODE;
        return head;
    }
}

/**
 * Look at first element from queue but don't dequeue
 * @param queue queue struct pointer
 * @return pointer to data of first node in the queue
 */
void* Queue_peek(Queue *queue) {
    pthread_mutex_lock(&queue->headLock);
    void* data = queue->head->data;
    if (queue->head == &queue->divider) {
        if (queue->head->next != NULL) {
            data = queue->head->next->data;
            pthread_mutex_unlock(&queue->headLock);
            return data;
        }
        else {
            pthread_mutex_unlock(&queue->headLock);
            return NULL;
        }
    }

    pthread_mutex_unlock(&queue->headLock);
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
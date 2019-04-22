#ifndef CS4204PRACTICAL2_QUEUE_H
#define CS4204PRACTICAL2_QUEUE_H
#include <pthread.h>

#define EOS -1

typedef struct queue_head {
    struct queue_head *next;
    void* data;
} QueueNode;

typedef struct queue_root {
    QueueNode* head;
    pthread_mutex_t headLock;
    QueueNode* tail;
    pthread_mutex_t tailLock;
    QueueNode divider;
    int size;
} Queue;

void* Queue_peek(Queue* queue);

void Queue_print(Queue *queue);

Queue* Queue_init();

void Queue_destroy(Queue* this);

void INIT_QUEUE_HEAD(QueueNode* head);

void Queue_put(QueueNode* node, Queue* queue);

QueueNode* Queue_get(Queue* queue);

#endif //CS4204PRACTICAL2_QUEUE_H

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
    pthread_mutex_t head_lock;

    QueueNode* tail;
    pthread_mutex_t tail_lock;

    QueueNode divider;

    int size;
} Queue;

void* Queue_peek(Queue* queue);

void Queue_print(Queue *queue);

Queue* Queue_init();

void INIT_QUEUE_HEAD(QueueNode* head);

void Queue_put(QueueNode* node, Queue* queue);

QueueNode* Queue_get(Queue* queue);

#endif //CS4204PRACTICAL2_QUEUE_H

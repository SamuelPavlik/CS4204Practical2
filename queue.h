#ifndef CS4204PRACTICAL2_QUEUE_H
#define CS4204PRACTICAL2_QUEUE_H

struct queue_root;

struct queue_head {
    struct queue_head *next;
    void* task;
};

struct queue_root *ALLOC_QUEUE_ROOT();
void INIT_QUEUE_HEAD(struct queue_head *head);

void queue_put(struct queue_head *new,
               struct queue_root *root);

struct queue_head *queue_get(struct queue_root *root);

#endif //CS4204PRACTICAL2_QUEUE_H

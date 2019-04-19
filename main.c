#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

#define MAX_THREAD_NUM 1000

pthread_t thread_ids[MAX_THREAD_NUM];
int count = 0;
struct queue_root* queueRoot;

void* svc(void* task) {
//    pthread_create(&thread_ids[count++], NULL, task)
    return NULL;
}

void* farm(void* task , int num) {
    return NULL;
}

void *printTask(void *toPrint) {
    printf("Printing: %s\n", ((char *) toPrint));
}

void *multiplyTask(void *toPrint) {
    printf("Multiplying: %d\n", ((int) toPrint)*2);

    return ((void*) toPrint);
}

int main() {
    queueRoot = ALLOC_QUEUE_ROOT();
    char* elem = "Element";
    void* status1;
    void* status2;
    struct queue_head* head1 = malloc(sizeof(struct queue_head));
    struct queue_head* head2 = malloc(sizeof(struct queue_head));

    head1->task = printTask;
    head2->task = multiplyTask;
    queue_put(head1, queueRoot);
    queue_put(head2, queueRoot);

    pthread_t* tid1 = malloc(sizeof(pthread_t));
    pthread_t* tid2 = malloc(sizeof(pthread_t));
    pthread_create(tid1, NULL, queue_get(queueRoot)->task, ((void*) "Hello"));
    pthread_create(tid2, NULL, queue_get(queueRoot)->task, ((void*) 10));
    pthread_join(*tid1, &status1);
    pthread_join(*tid2, &status2);
    printf("Both done\n");
//    printf("Status 1: %d\n", ((int) status1));
//    printf("Status 2: %d", ((int) status2));

    return 0;
}
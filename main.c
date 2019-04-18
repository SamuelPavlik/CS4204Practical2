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
    pthread_exit(NULL);
}

int main() {
    queueRoot = ALLOC_QUEUE_ROOT();
    char* elem = "Element";
    void* status;
    struct queue_head* head1 = malloc(sizeof(struct queue_head));
    struct queue_head* head2 = malloc(sizeof(struct queue_head));

    pthread_t* tid1 = malloc(sizeof(pthread_t));
    pthread_t* tid2 = malloc(sizeof(pthread_t));
    pthread_create(tid1, NULL, printTask, ((void*) "Elem1"));
    pthread_create(tid2, NULL, printTask, ((void*) "Elem2"));
    pthread_join(*tid1, &status);
    pthread_join(*tid2, &status);
    printf("Both done\n");

    return 0;
}
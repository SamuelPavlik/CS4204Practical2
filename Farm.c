#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"
#include "Worker.h"
#include "Farm.h"
#include "Pipeline.h"

/**
 * Initialize a farm struct
 * @param num number of workers
 * @param func function used by the workers
 * @param input input queue struct pointer
 * @param output output queue struct pointer
 * @return Farm struct pointer
 */
Farm* Farm_init(int num, void* (*func)(void*), Queue* input, Queue* output) {
    Farm* this = malloc(sizeof(Farm));
    this->base.name = FARM;
    this->n = num;
    this->workers = malloc(num * sizeof(Worker*));
    this->base.input = input;
    this->base.output = output;
    this->base.func = func;
    for (int i = 0; i < num; ++i) {
        this->workers[i] = Worker_init(func, this->base.input, this->base.output);
    }

    return this;
}

/**
 * Destroy a farm struct
 * @param this farm struct pointer
 */
Farm* Farm_destroy(Farm* this) {
//    if (this->base.input != NULL) {
//        free(this->base.input);
//        this->base.input = NULL;
//    }
//    if (this->base.output != NULL) {
//        free(this->base.output);
//        this->base.output = NULL;
//    }

    for (int i = 0; i < this->n; ++i) {
        Worker_destroy(this->workers[i]);
    }
    free(this->workers);
    free(this);

    return this;
}

/**
 * Run all workers of this farm
 * @param this farm struct pointer
 */
void Farm_run(Farm* this) {
//    printf("Running farm %p...\n", this);
    for (int i = 0; i < this->n; ++i) {
        Worker_run(this->workers[i]);
    }
}

/**
 * Wait for all workers to finish
 * @param this farm struct pointer
 */
void Farm_collect(Farm *this) {
    for (int i = 0; i < this->n; ++i) {
//        Worker* worker = this->workers[i];
//        printf("Join: %p\n", this->workers[i]->id);
        pthread_join(*(this->workers[i]->id), NULL);
    }
    Queue_put(Queue_get(this->base.input), this->base.output);
}

/**
 * Print farm info
 * @param this farm struct pointer
 */
void Farm_print(Farm *this) {
    printf("Farm\n");
    printf("Number of workers: %d\n", this->n);
    printf("Input: %p\n", this->base.input);
    printf("Output: %p\n", this->base.output);
    for (int i = 0; i < this->n; ++i) {
        printf("Worker: %p\n", this->workers[i]->id);
        printf("- status: %d\n", this->workers[i]->status);
    }
}

/**
 * Set input and output queues
 * @param this farm struct pointer
 */
void Farm_setWorkers(Farm *this) {
    for (int i = 0; i < this->n; ++i) {
        Worker_setPipes(this->workers[i], this->base.input, this->base.output);
    }
}

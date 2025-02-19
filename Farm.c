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
Farm* Farm_init(int num, void* (*func)(void*)) {
    Farm* this = malloc(sizeof(Farm));
    this->base.name = FARM;
    this->n = num;
    this->workers = malloc(num * sizeof(Worker*));
    this->base.func = func;
    for (int i = 0; i < num; ++i) {
        this->workers[i] = Worker_init(func, this->base.input, this->base.output, malloc(sizeof(pthread_t)));
    }

    return this;
}

/**
 * Destroy a farm struct
 * @param this farm struct pointer
 */
Farm* Farm_destroy(Farm* this) {
    Queue_destroy(this->base.input);
    this->base.input = NULL;

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
        printf("- input: %p\n", this->workers[i]->base.input);
        printf("- output: %p\n", this->workers[i]->base.output);
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

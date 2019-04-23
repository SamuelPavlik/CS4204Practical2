#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "Worker.h"
#include "Pipeline.h"

/**
 * Create worker struct
 * @param func worker function
 * @param input input queue struct pointer
 * @param output output queue struct pointer
 * @param id pthread id pointer
 * @return worker struct pointer
 */
Worker* Worker_init(void* (*func)(void*), Queue* input, Queue* output, pthread_t* id) {
    Worker* this = malloc(sizeof(Worker));
    this->base.name = STAGE;
    this->base.func = func;
    this->base.input = input;
    this->base.output = output;
    this->id = id;
    this->status = WAITING;

    return this;
}

/**
 * Destroy worker struct
 * @param this worker struct pointer
 */
void Worker_destroy(Worker* this) {
//    if (this->base.input != NULL) {
//        free(this->base.input);
//        this->base.input = NULL;
//    }
//    if (this->base.output != NULL) {
//        free(this->base.output);
//        this->base.output = NULL;
//    }

    free(this->id);
    free(this);
}

/**
 * Wrap around the original function and loop until EOS is reached
 * @param this worker struct pointer
 * @return EOS when done
 */
void* wrapperFunc(void* this) {
    Worker* wInput = ((Worker*) this);

    QueueNode* node = Queue_get(wInput->base.input);
    while(node != NULL) {
        //if node data is EOS enqueue back to the input queue and finish
        //this is in order to enqueue the EOS element when all workers are collected
        if (*((int *) node->data) == EOS) {
            Queue_put(node, wInput->base.input);
            break;
        }
        node->data = wInput->base.func(node->data);
        Queue_put(node, wInput->base.output);
        node = Queue_get(wInput->base.input);
    }

    return ((void*) EOS);
}

/**
 * Run this worker
 * @param this worker struct pointer
 */
void Worker_run(Worker* this) {
   //run on the main thread if no pthread id specified
    //it means that only worker is used in this stage
    if (this->id == NULL) {
        wrapperFunc(this);
    }
    //else create thread
    else {
        pthread_create(this->id, NULL, wrapperFunc, ((void *) this));
    }
}

/**
 * Wait for this worker to finish and cleanup if necessary
 * @param this worker struct pointer
 */
void Worker_collect(Worker* this) {
    //join if pthread id specified
    if (this->id != NULL) {
        pthread_join(*(this->id), NULL);
    }
    //else dequeue EOS node from the input queue
    else {
        Queue_put(Queue_get(this->base.input), this->base.output);
    }
}

/**
 * Set input and output queue of this worker struct
 * @param this worker struct pointer
 * @param input input queue struct pointer
 * @param output output queue struct pointer
 */
void Worker_setPipes(Worker *this, Queue *input, Queue *output) {
    this->base.input = input;
    this->base.output = output;
}

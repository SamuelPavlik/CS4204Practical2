#ifndef CS4204PRACTICAL2_PIPELINE_H
#define CS4204PRACTICAL2_PIPELINE_H

#include "Queue.h"
#include "Stage.h"

#define FARM "FARM"
#define STAGE "STAGE"
#define PIPE "PIPE"

typedef struct {
    Stage base;
    Stage** stages;
    int size;
//    Queue* input;
//    Queue* output;
} Pipeline;

Pipeline* Pipe_init();

void Pipe_addStage(Pipeline* pipeline, Stage* stage);

void Pipe_runPipe(Pipeline *pipeline);

void** Pipe_getOutput(Pipeline *pipeline);

void Pipe_putToQueue(Pipeline* pipeline, void* data[], int size);

long long Pipe_measure(Pipeline* pipeline);

long long* Pipe_measureFarm(int numOfWorkers, int maxIndex);

void Pipe_destroy(Pipeline* pipeline);

#endif //CS4204PRACTICAL2_PIPELINE_H

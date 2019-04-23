#ifndef CS4204PRACTICAL2_FARM_H
#define CS4204PRACTICAL2_FARM_H

#include "Worker.h"

typedef struct {
    Stage base;
//    char* name;
//    Queue* input;
//    Queue* output;
//    void* (*func)(void*);
    int n;
    Worker** workers;
} Farm;

Farm* Farm_init(int num, void* (*func)(void*));

Farm* Farm_destroy(Farm* this);

void Farm_setWorkers(Farm *this);

void Farm_run(Farm* this);

void Farm_print(Farm* this);

void Farm_collect(Farm* this);


#endif //CS4204PRACTICAL2_FARM_H

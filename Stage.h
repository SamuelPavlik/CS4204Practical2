#ifndef CS4204PRACTICAL2_STAGE_H
#define CS4204PRACTICAL2_STAGE_H

#include "Queue.h"

typedef struct {
    char* name;
    Queue* input;
    Queue* output;
    void* (*func)(void*);
} Stage;

#endif //CS4204PRACTICAL2_STAGE_H

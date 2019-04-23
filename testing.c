#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "parapat.h"

void *printTask(void *toPrint) {
    strcat(toPrint, "P");
//    printf("%s\n", ((char *) toPrint));
    return toPrint;
}

void* multiplyTask(void *toPrint) {
    strcat(toPrint, "M");
//    printf("%s\n", ((char *) toPrint));
    return toPrint;
}

/**
 * Test single farm pipeline for the output
 */
void testSingleFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputM") != 0) {
            printf("Test testSingleFarm failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testSingleFarm succeeded\n");
}

/**
 * Test two farms in pipeline for the output
 */
void testTwoFarms() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    Farm* farm2 = Farm_init(5, printTask);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_addStage(pipeline, (Stage *) farm2);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMP") != 0) {
            printf("Test testTwoFarms failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testTwoFarms succeeded\n");
}

/**
 * Test two farms and a worker in pipeline for output
 */
void testTwoFarmsAndWorker() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    Farm* farm2 = Farm_init(5, printTask);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_addStage(pipeline, (Stage *) farm2);
    Pipe_addStage(pipeline, (Stage *) worker1);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMPP") != 0) {
            printf("Test testTwoFarmsAndWorker failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testTwoFarmsAndWorker succeeded\n");
}

/**
 * Test worker and farm in pipeline for output
 */
void testWorkerAndFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) worker1);
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputPM") != 0) {
            printf("Test testWorkerAndFarm failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testWorkerAndFarm succeeded\n");
}

/**
 * Test farm, worker and another farm in pipeline for output
 */
void testFarmWorkerFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    Farm* farm2 = Farm_init(5, multiplyTask);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_addStage(pipeline, (Stage *) worker1);
    Pipe_addStage(pipeline, (Stage *) farm2);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMPM") != 0) {
            printf("Test testFarmWorkerFarm failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testFarmWorkerFarm succeeded\n");
}

/**
 * Test worker, farm and another worker in pipeline for output
 */
void testWorkerFarmWorker() {
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    Worker* worker2 = Worker_init(printTask, NULL, NULL, NULL);
    Farm* farm1 = Farm_init(5, multiplyTask);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }
    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) worker1);
    Pipe_addStage(pipeline, (Stage *) farm1);
    Pipe_addStage(pipeline, (Stage *) worker2);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputPMP") != 0) {
            printf("Test testWorkerFarmWorker failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testWorkerFarmWorker succeeded\n");
}

/**
 * Test farm followed by pipeline in pipeline for output
 */
void testFarmAndPipe() {
    Farm* farm2 = Farm_init(5, printTask);
    Farm* farm1 = Farm_init(5, multiplyTask);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }

    Pipeline* betweenPipe = Pipe_init();
    Pipe_addStage(betweenPipe, (Stage *) farm1);
    Pipe_addStage(betweenPipe, (Stage *) worker1);

    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) farm2);
    Pipe_addStage(pipeline, (Stage *) betweenPipe);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputPMP") != 0) {
            printf("Test testFarmAndPipe failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testFarmAndPipe succeeded\n");
}

/**
 * Test pipeline followed by farm in pipeline for output
 */
void testPipeAndFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    Farm* farm2 = Farm_init(5, printTask);
    int len = 5;
    void* data[len];
    for (int j = 0; j < len; ++j) {
        char* x = malloc(10 * sizeof(char));
        strcpy(x, "input");
        data[j] = x;
    }

    Pipeline* betweenPipe = Pipe_init();
    Pipe_addStage(betweenPipe, (Stage *) farm1);
    Pipe_addStage(betweenPipe, (Stage *) worker1);

    Pipeline* pipeline = Pipe_init();
    Pipe_addStage(pipeline, (Stage *) betweenPipe);
    Pipe_addStage(pipeline, (Stage *) farm2);
    Pipe_putToQueue(pipeline, data, len);
    Pipe_runPipe(pipeline);

    void** output = Pipe_getOutput(pipeline);
    Pipe_destroy(pipeline);
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMPP") != 0) {
            printf("Test testPipeAndFarm failed\n");
            return;
        }
    }
    //check if end of stream
    if (*((int *) output[len]) != EOS) {
        printf("Test testSingleFarm failed\n");
        return;
    }
    printf("Test testPipeAndFarm succeeded\n");
}

int main() {
    testSingleFarm();
    testTwoFarms();
    testTwoFarmsAndWorker();
    testWorkerAndFarm();
    testFarmWorkerFarm();
    testWorkerFarmWorker();
    testFarmAndPipe();
    testPipeAndFarm();

    return 0;
}
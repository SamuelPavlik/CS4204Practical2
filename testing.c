#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "parapat.h"
#include <assert.h>

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

void testSingleFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputM") != 0) {
            printf("Test testSingleFarm failed\n");
            return;
        }
    }
    printf("Test testSingleFarm succeeded\n");
}

void testTwoFarms() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
    Farm* farm2 = Farm_init(5, printTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMP") != 0) {
            printf("Test testTwoFarms failed\n");
            return;
        }
    }
    printf("Test testTwoFarms succeeded\n");
}

void testTwoFarmsAndWorker() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
    Farm* farm2 = Farm_init(5, printTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMPP") != 0) {
            printf("Test testTwoFarmsAndWorker failed\n");
            return;
        }
    }
    printf("Test testTwoFarmsAndWorker succeeded\n");
}

void testWorkerAndFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputPM") != 0) {
            printf("Test testWorkerAndFarm failed\n");
            return;
        }
    }
    printf("Test testWorkerAndFarm succeeded\n");
}

void testFarmWorkerFarm() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    Farm* farm2 = Farm_init(5, multiplyTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputMPM") != 0) {
            printf("Test testFarmWorkerFarm failed\n");
            return;
        }
    }
    printf("Test testFarmWorkerFarm succeeded\n");
}

void testWorkerFarmWorker() {
    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
    Worker* worker2 = Worker_init(printTask, NULL, NULL, NULL);
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
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
    for (int i = 0; i < len; ++i) {
        if (strcmp(output[i], "inputPMP") != 0) {
            printf("Test testWorkerFarmWorker failed\n");
            return;
        }
    }
    printf("Test testWorkerFarmWorker succeeded\n");
}



int main() {
    testSingleFarm();
    testTwoFarms();
    testTwoFarmsAndWorker();
    testWorkerAndFarm();
    testFarmWorkerFarm();
    testWorkerFarmWorker();

//    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
//    Worker* worker1 = Worker_init(printTask, NULL, NULL, NULL);
//    Farm* farm2 = Farm_init(5, printTask, NULL, NULL);
//    Worker* worker2 = Worker_init(printTask, NULL, NULL, NULL);
//    int len = 5;
//    void* data[len];
//    for (int j = 0; j < len; ++j) {
//        char* x = malloc(10 * sizeof(char));
//        strcpy(x, "s");
//        data[j] = x;
//    }

//    Pipeline* betweenPipe = Pipe_init();
//    Pipe_addStage(betweenPipe, (Stage *) farm1);
//    Pipe_addStage(betweenPipe, (Stage *) worker1);

//    Pipeline* pipeline = Pipe_init();
//    Pipe_addStage(pipeline, (Stage *) worker2);
//    Pipe_addStage(pipeline, (Stage *) farm2);
//    Pipe_addStage(pipeline, (Stage *) betweenPipe);
//    Pipe_addStage(pipeline, (Stage *) farm1);
//    Pipe_addStage(pipeline, (Stage *) worker1);
//    Pipe_putToQueue(pipeline, data, len);
//    Pipe_print(pipeline);
//    Pipe_runPipe(pipeline);

//    int size = pipeline->base.output->size - 1;
//    printf("Size: %d\n", size);
//    void** output = Pipe_getOutput(pipeline);
//    for (int i = 0; i < size; ++i) {
//        printf("Output %d: %s\n", i, ((char *) output[i]));
//    }

//    FILE* file = fopen("../results.txt", "a");
//    if (file == NULL)
//    {
//        printf("Error opening file!\n");
//        exit(1);
//    }
//
//    int maxIndex = 10;
//    for (int workers_num = 19; workers_num <= 20; workers_num+=1) {
//        printf("Number of workers: %d\n", workers_num);
//        long long* times = Pipe_measureFarm(workers_num, maxIndex);
//        for (int i = 0; i < maxIndex; ++i) {
////            if (i > 0) fprintf(file, ",");
////            fprintf(file, "%lld", times[i]);
//            printf("%lld,", times[i]);
//        }
////        fprintf(file, "\n");
//        printf("\n");
//        free(times);
//    }
//
//    fclose(file);

    return 0;
}
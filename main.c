#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(){
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
            break;
        }
    }
    printf("Test testFarmWorkerFarm succeeded\n");

    return 0;
}
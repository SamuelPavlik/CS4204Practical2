#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "Queue.h"
#include "Farm.h"
#include "Pipeline.h"
#include "Stage.h"

void *printTask(void *toPrint) {
//    int* num = ((int*) toPrint);
    strcat(toPrint, "P");
    printf("Printing: %s\n", ((char *) toPrint));
    return toPrint;
}

void* multiplyTask(void *toPrint) {
//    int* num = ((int*) toPrint);
    strcat(toPrint, "M");
    printf("Multiplying: %s\n", ((char *) toPrint));
    return toPrint;
}

//int fib(int n)
//{
//    int  i, Fnew, Fold, temp,ans;
//
//    Fnew = 1;  Fold = 0;
//    for ( i = 2;
//          i <= n;          /* apsim_loop 1 0 */
//          i++ )
//    {
//        temp = Fnew;
//        Fnew = Fnew + Fold;
//        Fold = temp;
//    }
//    ans = Fnew;
//    return ans;
//}
//
//void* fibFunc(void* n) {
//    int* result = malloc(sizeof(int));
//    *result = fib(*((int *) n));
//    printf("Result: %d\n", *result);
//    return result;
//}


int main() {
    Farm* farm1 = Farm_init(5, multiplyTask, NULL, NULL);
    Worker* worker1 = Worker_init(printTask, NULL, NULL);
    Farm* farm2 = Farm_init(5, printTask, NULL, NULL);
    Worker* worker2 = Worker_init(printTask, NULL, NULL);
//    int len = 50000;
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
//    Pipe_addStage(pipeline, (Stage *) farm1);
//    Pipe_addStage(pipeline, (Stage *) worker1);
//    Pipe_putToQueue(pipeline, data, len);
//    Pipe_runPipe(pipeline);

//    int size = pipeline->base.output->size - 1;
//    printf("Size: %d\n", size);
//    void** output = Pipe_getOutput(pipeline);
//    for (int i = 0; i < size; ++i) {
//        printf("Output %d: %s\n", i, ((char *) output[i]));
//    }

    FILE* file = fopen("../results.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int maxIndex = 20;
    for (int workers_num = 19; workers_num <= 20; workers_num+=1) {
        printf("Number of workers: %d\n", workers_num);
        long long* times = Pipe_measureFarm(workers_num, maxIndex);
        for (int i = 0; i < maxIndex; ++i) {
//            if (i > 0) fprintf(file, ",");
//            fprintf(file, "%lld", times[i]);
            printf("%lld,", times[i]);
        }
//        fprintf(file, "\n");
        printf("\n");
        free(times);

    }

//    long long* diffs = Pipe_measureFarm(21, maxIndex);
//    for (int i = 0; i < maxIndex; ++i) {
////        if (i > 0) fprintf(file, ",");
////        fprintf(file, "%lld", diffs[i]);
//        printf("%lld,", diffs[i]);
//    }
////        fprintf(file, "\n");
//    printf("\n");
//    free(diffs);

    fclose(file);

    return 0;
}
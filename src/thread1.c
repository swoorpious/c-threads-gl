#include "core.h"
#include <pthread.h>
#include "thread1.h"

#include <math.h>


void* someFunc(void* args) {
    const auto t = (threadArgs*) args;
    testData *td = t->td;
    timeStamp *ts = t->ts;

    struct timespec threadTS;
    timespec_get(&threadTS, TIME_UTC);
    ts->start_time = threadTS.tv_nsec;
    
    td->one += td->one;
    td->two += td->two;
    td->three += td->three;

    const pthread_t id = pthread_self();
    printf("running thread id: %lu\n", (unsigned long)id);
    
    timespec_get(&threadTS, TIME_UTC);
    ts->end_time = threadTS.tv_nsec;
    return t;
}

int main() {
    printf("running LearningThreads\n");
    
    timeStamp mainThreadTime;
    struct timespec mainThreadTS;
    // mainThread start time
    timespec_get(&mainThreadTS, TIME_UTC);
    mainThreadTime.start_time = mainThreadTS.tv_nsec;

    timeStamp threadTime = {0};
    
    testData data;
    data.one = 1;
    data.two = 2;
    data.three = 3;


    threadArgs args;
    args.td = &data;
    args.ts = &threadTime;
    pthread_t thread1;
    pthread_create(&thread1, NULL, someFunc, (void*) &args);

    timespec_get(&mainThreadTS, TIME_UTC);
    mainThreadTime.intermediate_time = mainThreadTS.tv_nsec;    
    printf("one = %d\ntwo = %d\nthree = %d\n\n", data.one, data.two, data.three);
    
    pthread_join(thread1, NULL);
    printf("joined thread id: %lu\n", (unsigned long)thread1);
    printf("one = %d\ntwo = %d\nthree = %d\n\n", data.one, data.two, data.three);

    // mainThread end time
    timespec_get(&mainThreadTS, TIME_UTC);
    mainThreadTime.end_time = mainThreadTS.tv_nsec;

    printf("time details;\n");
    printf("main thread started at: %luns\n", mainThreadTime.start_time);
    printf("main thread intermediate at: %luns\n", mainThreadTime.intermediate_time);
    printf("main thread ended at: %luns\n", mainThreadTime.end_time);
    printf("main thread took %fms to complete\n\n", (double)(mainThreadTime.end_time - mainThreadTime.start_time) / 1e6);
    printf("child thread id (%lu) started at: %luns\n", (unsigned long)thread1, threadTime.start_time);
    printf("child thread id (%lu) ended at: %luns\n", (unsigned long)thread1, threadTime.end_time);
    printf("child thread id (%lu) took %fms to complete\n\n", (unsigned long)thread1, (double)(threadTime.end_time - threadTime.start_time) / 1e6);
    const double del = labs((long)(mainThreadTime.intermediate_time - threadTime.start_time)) / 1e6; // delta between starting
    printf("delta between main thread and child thread race condition: %fms\n", del);
    
	return 0;
}

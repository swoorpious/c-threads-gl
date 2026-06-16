// copyright 2026 swaroop.

#ifndef LEARNINGTHREADS_THREAD1_H
#define LEARNINGTHREADS_THREAD1_H

typedef struct {
    int one;
    int two;
    int three;
} testData;

typedef struct {
    unsigned long start_time;
    unsigned long intermediate_time; // used for anything to check in between
    unsigned long end_time;
} timeStamp;

typedef struct {
    timeStamp *ts;
    testData *td;
} threadArgs;

#endif //LEARNINGTHREADS_THREAD1_H
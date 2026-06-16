// copyright 2026 swaroop.
#include "core.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 8
#define BUFFER_SIZE 10

pthread_mutex_t mutexBuffer;
sem_t semEmpty;
sem_t semFull;

int buffer[BUFFER_SIZE] = {0};
int count = 0;

void *producer(void *arg) {
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    srand(seed);
    while (1) {
        int x = rand() % 100;
        sleep(1);
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count++] = x;
        printf("added: %d (%lu)\n", x, (unsigned long)pthread_self());
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void *consumer(void *arg) {
    while (1) {
        int y = -1;
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        y = buffer[--count];
        buffer[count] = -1;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);
        printf("consumed: %d (%lu)\n", y, (unsigned long)pthread_self());
        
    }
}


int main (int argc, char* argv[]) {
    printf("running executable: %s\n\n", argv[0]);
    
    // srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFER_SIZE); // initial value to be BUFFER_SIZE
    sem_init(&semFull, 0, 0);

    // create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&threads[i], NULL, &producer, NULL))
                perror("pthread_create failed to create thread for &producer");
        }
        else {
            if (pthread_create(&threads[i], NULL, &consumer, NULL))
                perror("pthread_create failed to create thread for &consumer");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL))
            perror("pthread_join failed");
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    
    return 0;
}
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include "bbuff.h"

sem_t fullBuffer;
sem_t emptyBuffers;
sem_t mutex;
void* buffer_data[BUFFER_SIZE];
int buffer_count;

void bbuff_init(void) {
    sem_init(&fullBuffer, 0, 0);
    sem_init(&emptyBuffers, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1);
    buffer_count = 0;
}

void bbuff_blocking_insert(void* item) {
    // while(true) {
        sem_wait(&emptyBuffers);
        sem_wait(&mutex);
        buffer_data[buffer_count++] = item;
        sem_post(&mutex);
        sem_post(&fullBuffer);
    // }
}

void* bbuff_blocking_extract(void) {
    // while(true) {
        sem_wait(&fullBuffer);
        sem_wait(&mutex);
        void* candy = &buffer_data[buffer_count--];
        sem_post(&mutex);
        sem_post(&emptyBuffers);
        return candy;
    // }
}

_Bool bbuff_is_empty(void) {
    if(buffer_count == 0) {
        return true;
    } else {
        return false;
    }
}
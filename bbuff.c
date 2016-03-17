#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include "bbuff.h"

sem_t full_buffer;
sem_t empty_buffers;
sem_t mutex;
void* buffer_data[BUFFER_SIZE];
int buffer_count;

void bbuff_init(void) {
    sem_init(&full_buffer, 0, 0);
    sem_init(&empty_buffers, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1);
    buffer_count = 0;
}

void bbuff_blocking_insert(void* item) {
    int count = 0;
    sem_wait(&empty_buffers);
    sem_wait(&mutex);
    sem_getvalue(&full_buffer, &count);
    buffer_data[count] = item;
    sem_post(&mutex);
    sem_post(&full_buffer);
}

void* bbuff_blocking_extract(void) {
    int count = 0;
    sem_wait(&full_buffer);
    sem_wait(&mutex);
    sem_getvalue(&full_buffer, &count);
    void* candy = buffer_data[count];
    sem_post(&mutex);
    sem_post(&empty_buffers);
    return candy;
}

_Bool bbuff_is_empty(void) {
    int count = -1;
    sem_getvalue(&full_buffer, &count);
    if(count == 0) {
        return true;
    } else {
        return false;
    }
}
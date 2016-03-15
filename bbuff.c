#include <stdbool.h>
#include <stdlib.h>
#include "bbuff.h"

// Semaphore fullBuffer = 0;
// Semaphore emptyBuffers = numBuffers;
//
// Semaphore mutex = 1;

void* buffer_data[BUFFER_SIZE];

void bbuff_init(void) {
    
}

void bbuff_blocking_insert(void* item) {

}

void* bbuff_blocking_extract(void) {
    return NULL;
}

_Bool bbuff_is_empty(void) {
    return false;
}
// Producer(item) {
//     emptyBuffers.P();
//     mutex.P();
//     Enqueue(item);
//     mutex.V();
//     fullBuffers.V();
// }
//
// Consumer() {
//     fullBuffers.P();
//     mutex.P();
//     item = Dequeue();
//     mutex.V();
//     emptyBuffers.V();
//     return item;
// }
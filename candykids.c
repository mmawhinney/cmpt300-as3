#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
// #include <string.h>
#include "bbuff.h"
#include "stats.h"


_Bool stop_thread = false;

struct candy_t {
    int factory_number;
    double time_stamp_in_ms;
};

double current_time_in_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

void insert_candy(int factory_num) {
    struct candy_t* candy = malloc(sizeof(struct candy_t));
    candy->factory_number = factory_num;
    candy->time_stamp_in_ms = current_time_in_ms();
    bbuff_blocking_insert(candy);
}

void* factory_thread_func(void* arg) {
    while(!stop_thread) {
        // TODO: pick random num between 0 and 3
        // TODO: change 0 to the ID and 2s to the random number
        write(STDOUT_FILENO, "\tFactory 0 ships candy & waits 2s\n", 34);
        // TODO: change argument to id
        insert_candy(0);
        // TODO: change argument to random num from above
        sleep(2);
    }
    printf("Candy-factory 0 done\n");
    return NULL;
}

void* kid_thread_func(void* arg) {
    while(true) {
        struct candy_t* candy = (struct candy_t*)bbuff_blocking_extract();
        int factory_id = candy->factory_number;
        double timestamp = candy->time_stamp_in_ms;
        printf("Candy - Factory ID: %d, Timestamp: %f\n", factory_id, timestamp);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    /*
    * extract arguments
    */
    if(argc < 4) {
        printf("Usage: ./candykids <#factories> <#kids> <#seconds>\n");
        exit(1);
    }
        
    int num_factories = atoi(argv[1]);
    int num_kids = atoi(argv[2]);
    int num_seconds= atoi(argv[3]);
    
    if(num_factories <= 0 || num_kids <= 0 || num_seconds <= 0) {
        printf("All arguments must be greater than 0\n");
        exit(1);
    }
    
    /*
    * initialize bounded buffer and stats modules
    */
    bbuff_init();
    // stats_init(num_producers);

    /*
    * launch factory threads
    */
    pthread_t factory_thread_ids[num_factories];
    
    for(int i = 0; i < num_factories; i++) {
        pthread_t factory_id;
        pthread_create(&factory_id, NULL, &factory_thread_func, NULL);
        if(errno < 0) {
            perror("Error creating factory thread\n");
        }
        factory_thread_ids[i] = factory_id;
    }    
    
    /*
    * launch kid threads
    */
    pthread_t kid_thread_ids[num_kids];
    for(int i = 0; i < num_kids; i++) {
        pthread_t kid_id;
        pthread_create(&kid_id, NULL, &kid_thread_func, NULL);
        if(errno < 0) {
            perror("Error creating kid thread\n");
        }
        kid_thread_ids[i] = kid_id;
    }
    
    /*
    * wait
    */
    for(int i = 0; i < num_seconds; i++) {
        printf("Time %ds\n", i);
        sleep(1);   
    }
    
    /*
    * stop factory threads
    */
    stop_thread = true;
    for(int i = 0; i < num_factories; i++) {
        pthread_t id = factory_thread_ids[i];
        pthread_cancel(id);
        pthread_join(id, NULL);
    }
    
    /*
    * wait for no candy left
    */
    while(bbuff_is_empty() == false) {
        printf("Waiting for all candy to be consumed\n");
        sleep(1);
    }
    
    /*
    * stop kid threads
    */
    for(int i = 0; i < num_kids; i++) {
        pthread_t id = kid_thread_ids[i];
        pthread_cancel(id);
        pthread_join(id, NULL);
    }
    
    /*
    * print stats
    */

    /*
    * deallocate memory
    */

}
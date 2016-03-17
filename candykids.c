#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
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
    return (now.tv_sec * 1000.0) + (now.tv_nsec/1000000.0);
}

void insert_candy(int factory_num) {
    struct candy_t* candy = malloc(sizeof(struct candy_t));
    candy->factory_number = factory_num;
    candy->time_stamp_in_ms = current_time_in_ms();
    bbuff_blocking_insert(candy);
}

void* factory_thread_func(void* arg) {
    int* id = (int*)arg;
    while(!stop_thread) {
        // pick random num between 0 and 3
        int sleep_time = rand() % 4;
        printf("\tFactory %d ships candy & waits %ds\n", *id, sleep_time);
        insert_candy(*id);
        // TODO: increment candy-created count
        stats_record_produced(*id);
        sleep(sleep_time);
    }
    printf("Candy-factory %d done\n", *id);
    free(id);
    return NULL;
}

void* kid_thread_func(void* arg) {
    while(true) {
        struct candy_t* candy = bbuff_blocking_extract();
        int factory_id = candy->factory_number;
        double timestamp = candy->time_stamp_in_ms;
        printf("Candy - Factory ID: %d, Timestamp: %f\n", factory_id, timestamp);
        stats_record_consumed(factory_id, timestamp);
        free(candy);
        // pick random num, either 0 or 1
        int sleep_time = rand() % 2;
        sleep(sleep_time);
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
    
    srand(time(NULL));
    
    /*
    * initialize bounded buffer and stats modules
    */
    bbuff_init();
    stats_init(num_factories);
    
    /*
    * launch factory threads
    */
    pthread_t factory_thread_ids[num_factories];
    
    for(int i = 0; i < num_factories; i++) {
        int* id = malloc(sizeof(*id));
        *id = i;
        pthread_t factory_id;
        pthread_create(&factory_id, NULL, &factory_thread_func, id);
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
    stats_display();
    

    /*
    * deallocate memory
    */
    stats_cleanup();

}
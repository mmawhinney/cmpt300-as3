#include "stats.h"
#include <stdlib.h>

struct factory_stats {
    int factory;
    int made;
    int eaten;
    int minDelay;
    int avgDelay;
    int maxDelay;
};

int num_factories;
struct factory_stats** stats_array;

void stats_init(int num_producers) {
    num_factories = num_producers;
    stats_array = malloc(sizeof(struct factory_stats) * num_producers);
    for(int i = 0; i < num_producers; i++) {
        struct factory_stats* stats = malloc(sizeof(struct factory_stats));
        stats_array[i] = stats;
    }
}

void stats_cleanup(void) {
    for(int i = 0; i < num_factories; i++) {
        free(stats_array[i]);
    }
    free(stats_array);
}

void stats_record_produced(int factory_number) {
    
}

void stats_record_consumed(int factory_number, double delay_in_ms) {
    
}

void stats_display(void) {
    
}
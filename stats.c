#include "stats.h"
#include <stdlib.h>

typedef struct {
    int factory;
    int made;
    int eaten;
    int minDelay;
    int avgDelay;
    int maxDelay;
} factory_stats;

int num_factories;
factory_stats* stats_array;

void stats_init(int num_producers) {
    stats_array = malloc(sizeof(factory_stats) * num_producers);
}

void stats_cleanup(void) {
    free(stats_array);
}

void stats_record_produced(int factory_number) {
    
}

void stats_record_consumed(int factory_number, double delay_in_ms) {
    
}

void stats_display(void) {
    
}
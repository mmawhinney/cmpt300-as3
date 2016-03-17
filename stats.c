#include "stats.h"
#include <stdlib.h>
#include <stdio.h>

struct factory_stats {
    int factory;
    int made;
    int eaten;
    double min_delay;
    double total_delay;
    double avg_delay;
    double max_delay;
};

int num_factories;
struct factory_stats** stats_array;

void stats_init(int num_producers) {
    num_factories = num_producers;
    stats_array = malloc(sizeof(struct factory_stats) * num_producers);
    for(int i = 0; i < num_producers; i++) {
        struct factory_stats* stats = malloc(sizeof(struct factory_stats));
        stats->factory = i;
        stats->made = 0;
        stats->eaten = 0;
        stats->min_delay = 0.0;
        stats->max_delay = 0.0;
        stats->total_delay = 0.0;
        stats->avg_delay = 0.0;
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
    struct factory_stats* stats = stats_array[factory_number];
    stats->made = stats->made + 1;
}

void stats_record_consumed(int factory_number, double delay_in_ms) {
    struct factory_stats* stats = stats_array[factory_number];
    
    stats->eaten = stats->eaten + 1;
    double min = stats->min_delay;
    if(min == 0.0 || delay_in_ms < min) {
        stats->min_delay = delay_in_ms;
    }
    
    double max = stats->max_delay;
    if(delay_in_ms > max) {
        stats->max_delay = delay_in_ms;
    }
    
    double total_delay = stats->total_delay + delay_in_ms;
    stats->total_delay = total_delay;
    stats->avg_delay = total_delay / stats->eaten;
}

void stats_display(void) {
    printf("%8s%8s%8s%15s%15s%15s\n", "Factory#", "#Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
    for(int i = 0; i < num_factories; i++) {
        struct factory_stats* stats = stats_array[i];
        if(stats->eaten != stats->made) {
            printf("ERROR: Mismatch between number made and eaten\n");
            return;
        }
        printf("%8d%8d%8d%15.5f%15.5f%15.5f\n", stats->factory, stats->made, stats->eaten, stats->min_delay, stats->avg_delay, stats->max_delay);
    }
    
}
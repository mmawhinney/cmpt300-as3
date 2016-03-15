#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "bbuff.h"


typedef struct {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;

double current_time_in_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

int main(int argc, char* argv[]) {
    
    if(argc < 4) {
        printf("Usage: ./candykids <#factories> <#kids> <#seconds>\n");
        exit(1);
    }
        
    int numFactories = atoi(argv[1]);
    int numKids = atoi(argv[2]);
    int numSeconds= atoi(argv[3]);
    
    if(numFactories <= 0 || numKids <= 0 || numSeconds <= 0) {
        printf("All arguments must be greater than 0\n");
        exit(1);
    }
    

    
    

}
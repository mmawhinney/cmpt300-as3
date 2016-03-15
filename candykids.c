#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




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
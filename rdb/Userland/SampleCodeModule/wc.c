#include <wc.h>

void start_wc(){
    uint64_t counter = 1;
    char * c = malloc(1);
    while(read(c,1) != 0){
        if(*c == '\n'){
            counter++;
        }
    }
    printf("input lines: %d\n",counter);
}
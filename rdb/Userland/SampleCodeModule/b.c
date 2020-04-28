#include <b.h>

void start_b(void){
    int i = 0;
    while(1){
        if(i%100 == 0){
            printf("B is counting: %d\n", i);
        }
        i++;
    }
}
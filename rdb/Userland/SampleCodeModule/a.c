#include <a.h>

void start_a(void){
    for (size_t i = 0; i < 100000; i++){ 
        wait(1);
        printf("A is counting: %d\n", i);
    }
}
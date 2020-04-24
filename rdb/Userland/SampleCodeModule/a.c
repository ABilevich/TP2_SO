#include <a.h>

void start_a(void){
    for (size_t i = 0; i < 100000; i++){ 
        wait(1);
        if(i == 3){
            kill(2);
            printProcessInfo(2);
        }
        printf("A is counting: %d\n", i);
    }
}
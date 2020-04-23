#include <a.h>

void start_a(void){
    for (size_t i = 0; i < 100000; i++){ 
        wait(1);
        if(i == 3){
            changeProcessState(2,BLOCKED);
            printProcessInfo(2);

        } else if (i == 7){
            changeProcessState(2,READY);
            printProcessInfo(2);
        }
        printf("A is counting: %d\n", i);
    }
}
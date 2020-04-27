#include <loop.h>

void start_loop(void){
    while(1){
        wait(1);
        printf("Loop PID: %d\n", getPid());
    }
}
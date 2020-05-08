#include <c.h>

void start_c(void){

    int *variable = (void*)0xF00000;
    int old;
    sem_info * si = semOpen("pepito", 1);
    int i = 0;
    while( i < 1000){
        semWait(si);

        old = *variable;
        old--;
        *variable = old;
        
        i++;
        semPost(si);
    }

    printf("holaaaa\n");

    semClose(si);

    printf("B final counter: %d\n", *variable);

    wait(1);
    printf("end\n");
}
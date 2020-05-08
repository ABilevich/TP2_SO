#include <c.h>

void start_c(void){

    uint64_t * counter = (void*)0xE00000;
    // int i = 0;
    // while(1){
    //     if(i%100 == 0){
    //         printf("B is counting: %d\n", i);
    //     }
    //     i++;
    // }



    int sem = semOpen("pepito", 1);
    int i = 0;
    while( i < 10){
        semWait(sem);
        
        *counter = *counter - 1;
         printf("C count down\n");
        semPost(sem);
        i++;
    }

    semClose(sem);
    semUnlink("pepito");
    printf("C final counter: %d\n", *counter);
    //printf("sem id after unlink: %d\n", sem);
    //printf("semOpen\n");
    //int sem2 = semOpen("juan", 1);

    //while(1);
    printf("end\n");
}
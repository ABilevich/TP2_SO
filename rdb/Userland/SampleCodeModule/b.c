#include <b.h>

void start_b(void){

    uint64_t * counter = (void*)0xF000000;


    // int i = 0;
    // while(1){
    //     if(i%100 == 0){
    //         printf("B is counting: %d\n", i);
    //     }
    //     i++;
    // }


    printf("B semOpen\n");
    int sem = semOpen("pepito", 1);
    int i = 0;
    while( i < 100000){
        printf("B semWait\n");
        semWait(sem);

        *counter++;

        printf("B semPost\n");
        semPost(sem);
        i++;
    }

    printf("B semClose\n");
    semClose(sem);

    printf("B semUnlink\n");
    semUnlink("pepito");

    //printf("sem id after unlink: %d\n", sem);

    printf("B counter: %d\n", *counter);

    // printf("semOpen\n");
    // int sem2 = semOpen("juan", 1);

    while(1);
    printf("end\n");  

}
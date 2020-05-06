#include <c.h>

void start_c(void){

    uint64_t * counter = (void*)0xF000000;
    // int i = 0;
    // while(1){
    //     if(i%100 == 0){
    //         printf("B is counting: %d\n", i);
    //     }
    //     i++;
    // }


    printf("C: semOpen\n");

    int sem = semOpen("pepito", 1);
    int i = 0;
    while( i < 100000){
        printf("C: semWait\n");
        semWait(sem);

        *counter--;

        printf("C: semPost\n");
        semPost(sem);
        i++;
    }

    printf("C:semClose\n");
    semClose(sem);

    printf("C: semUnlink\n");
    semUnlink("pepito");

    //printf("sem id after unlink: %d\n", sem);


    // printf("semOpen\n");
    // int sem2 = semOpen("juan", 1);

    while(1);
    printf("end\n");
    

}
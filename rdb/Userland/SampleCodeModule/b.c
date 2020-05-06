#include <b.h>
#include <c.h>
 
void start_b(void){

    uint64_t * counter = (void*)0xF000000;

    void (*c)(void);
    c = &start_c;
    createProcess(c, 2, 0, "c");

    //while(1);

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
        //semWait(sem);
        printf("B ");
        *counter = *counter + 1;

        //semPost(sem);
        printf("Repeat\n");
        i++;
    }

    semClose(sem);

    semUnlink("pepito");

    //printf("sem id after unlink: %d\n", sem);

    printf("B counter: %d\n", *counter);

    // printf("semOpen\n");
    // int sem2 = semOpen("juan", 1);

    while(1);
    printf("end\n");  

}
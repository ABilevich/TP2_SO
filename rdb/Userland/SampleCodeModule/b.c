#include <b.h>
#include <c.h>
 
void start_b(void){

    // uint64_t * counter = (void*)0xE00000;

    void (*c)(void);
    c = &start_c;
    createProcess(c, 2, 0, "c",0,0);

    
    //while(1);

    // int i = 0;
    // while(1){
    //     if(i%100 == 0){
    //         printf("B is counting: %d\n", i);
    //     }
    //     i++;
    // }
   
    int sem = semOpen("pepito", 1);
//     // int i = 0;
//     // while( i < 10){
    semWait(sem);

        wait(2);
//     //     *counter = *counter + 1;
//     //     printf("B count up\n");
    semPost(sem);
//     //     i++;
//     // }

    semClose(sem);

//     semUnlink("pepito");

//     //printf("sem id after unlink: %d\n", sem);

//     printf("B final counter: %d\n", *counter);

//     // printf("semOpen\n");
//     // int sem2 = semOpen("juan", 1);

//    // while(1);
    wait(1);
    printf("end\n");
}
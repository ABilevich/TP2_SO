#include <b.h>

void start_b(void){
    // int i = 0;
    // while(1){
    //     if(i%100 == 0){
    //         printf("B is counting: %d\n", i);
    //     }
    //     i++;
    // }
    printf("semOpen\n");
    int sem = semOpen("pepito", 1);
    printf("semWait\n");
    semWait(sem);
    printf("semPost\n");
    semPost(sem);
    printf("semUnlink\n");
    semUnlink("pepito");
    printf("semClose\n");    
    semClose(sem);
    while(1);
    printf("end\n");
    
}
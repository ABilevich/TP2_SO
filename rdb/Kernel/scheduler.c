#include <scheduler.h>

int next_pid = 0;
    
typedef struct pcb{
    void * rsp;
    size_t pid;	
    size_t priority;
} pcb;

typedef struct node{
    pcb * process;
    struct node * next;			
} node;
    

void * scheduler(void * resp){


    return resp;
}


int addPCB(void * rsp, size_t priority){
   return 0; 
}
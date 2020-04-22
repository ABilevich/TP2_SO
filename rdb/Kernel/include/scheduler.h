#ifndef _SCHEDUL
#define _SCHEDUL

#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>

typedef struct s_pcb{
    void * rsp;
    void * stack_start;
    uint64_t pid;	
    uint64_t priority;
    uint64_t p_counter;
    char is_deletable;
} s_pcb;

typedef struct s_node{
    s_pcb * pcb;
    struct s_node * next;	
    struct s_node * prev;	
} s_node;

void * scheduler(void * old_rsp);
int addPCB(void * rsp, size_t priority, void * stack_start);
void init(s_pcb * new_pcb);
void addProcess(s_pcb * new_pcb);
int kill(uint64_t pid);
void printPCB(s_pcb * pcb);


#endif

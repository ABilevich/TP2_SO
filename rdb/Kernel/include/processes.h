#include <stdint.h>
#include <stddef.h>
#include <scheduler.h>
#include <mm_interface.h>

typedef void (*fn)();

int sys_process(void * option, void * arg1, void * arg2, void * arg3, void * arg4);
void p_createProcess(void * rip, uint64_t priority, char fg, char * name);
void p_changeProcessPriority(uint64_t pid, uint64_t priority, int * resp);
void p_changeProcessState(uint64_t pid,char state, int * resp);
void p_kill(uint64_t pid, int * resp);
void p_killCurrent();
void p_printProcessInfo(uint64_t pid);
void p_printAllProcessInfo(void);
void p_getPid( int * resp);
void wrapper(int argc, char * argv[], fn to_be_run);
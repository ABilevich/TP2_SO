#include <lib_user.h>

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3

uint64_t my_getpid();
uint64_t prio_create_process(char * name);
uint64_t my_nice(uint64_t pid, uint64_t newPrio);
uint64_t my_prio_kill(uint64_t pid);
uint32_t my_prio_block(uint32_t pid);
uint32_t my_prio_unblock(uint32_t pid);
void bussy_wait(uint64_t n);
void prio_endless_loop();


void test_prio();
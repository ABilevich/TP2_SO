#include <stdint.h>
#include <stddef.h>
#include <scheduler.h>
#include <mm_interface.h>

void * createProcess(void * rip, uint64_t priority, char fg);
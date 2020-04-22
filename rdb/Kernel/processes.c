#include <processes.h>


int sys_process(void * option, void * arg1, void * arg2, void * arg3) {
    switch ((uint64_t) option) {
	case 0:
		createProcess((void *) arg1, (uint64_t *) arg2);
		break;
	return 0;
	}
}

void createProcess(void * rip, uint64_t priority){

    printString("New Process: ", 13);
    printNewLine();
    printString("rip: ", 5);
	print64Hex((uint64_t)rip);
	printNewLine();
    printString("priority: ", 10);
	printDec(priority);
	printNewLine();
	
}

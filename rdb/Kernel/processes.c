#include <processes.h>

#define STACK_SIZE 512

int sys_process(void * option, void * arg1, void * arg2, void * arg3, void * arg4) {
    switch ((uint64_t) option) {
	case 0:
		p_createProcess((void *) arg1, (uint64_t)arg2, (char)arg3, (char *) arg4);
		break;
	case 1:
		p_changeProcessPriority((uint64_t)arg1, (uint64_t)arg2, (int*)arg3);
		break;
	case 2:
		p_changeProcessState((uint64_t)arg1, (char)arg2, (int*)arg3);
		break;
	case 3:
		p_kill((uint64_t)arg1, (int*)arg2);
		break;
	case 4:
		p_printProcessInfo((uint64_t)arg1);
		break;
	case 5:
		p_printAllProcessInfo();
		break;
	case 6:
		p_getPid((int*)arg1);
		break;
	}
	return 0;
}

void p_createProcess(void * rip, uint64_t priority, char fg, char * name){

	// printString("New Process: ", 13);
    // printNewLine();
	void * stack_start;
	void * bp;
	malloc(STACK_SIZE, &stack_start);

	// printString("stack start: ", 13);
	// print64Hex((uint64_t)stack_start);
	// printNewLine();

	bp =  (void *) ( (uint64_t * )stack_start + STACK_SIZE + 1);

	// printString("base pointer: ", 14);
	// print64Hex((uint64_t)bp);
	// printNewLine();
	
	void * rsp = (void*)( (uint64_t *)bp - 20 );

	void * argc = 0;
	void * argv = 0xABCD;

	_fillstack(bp, rip, argc, argv);

	addPCB(rsp , priority, stack_start, fg, name);


	// for(int i = 0; i < 30; i++){
	// 	printString("when i= ", 8);
	// 	printDec(i);
	// 	printString(" rsp = ", 7);
	// 	print64Hex( (uint64_t *)rsp - i );
	// 	printString(" has: ", 6);
	// 	print64Hex( * ( (uint64_t *)rsp - i ) );
	// 	printNewLine();
	// }
	
	// printString("stack pointer: ",15);
	// print64Hex( (uint64_t)rsp );
	// printNewLine();

	// printString("rip: ", 5);
	// print64Hex((uint64_t)rip);
	// printNewLine();
    // printString("priority: ", 10);
	// printDec(priority);
	// printNewLine();

	return;
}

void p_changeProcessPriority(uint64_t pid, uint64_t priority, int * resp){
	*resp = changePriority(pid, priority);
	return;
}

void p_changeProcessState(uint64_t pid,char state, int * resp){
	*resp = changeState(pid, state);
	return;
}

void p_kill(uint64_t pid, int * resp){
	*resp = kill(pid);
	return;
}

void p_printProcessInfo(uint64_t pid){
	printProcessInfo(pid);
	return;
}

void p_printAllProcessInfo(){
	printAllProcessInfo();
	return;
}

void p_getPid(int * resp){
	* resp = getCurrentPid();
	return;
}
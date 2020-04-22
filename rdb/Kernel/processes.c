#include <processes.h>

#define STACK_SIZE 512

int sys_process(void * option, void * arg1, void * arg2, void * arg3) {
    switch ((uint64_t) option) {
	case 0:
		createProcess((void *) arg1, (uint64_t) arg2, (char) arg3);
		break;
	return 0;
	}
}

void * createProcess(void * rip, uint64_t priority, char fg){

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

	addPCB(rsp , priority, stack_start, fg);


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

	return rsp;
}

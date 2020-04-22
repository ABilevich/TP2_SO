#include <processes.h>

#define STACK_SIZE 512

int sys_process(void * option, void * arg1, void * arg2, void * arg3) {
    switch ((uint64_t) option) {
	case 0:
		createProcess((void *) arg1, (uint64_t) arg2);
		break;
	return 0;
	}
}

void createProcess(void * rip, uint64_t priority){

	printString("New Process: ", 13);
    printNewLine();
	void * stack_start;
	void * rsp;
	malloc(STACK_SIZE, &stack_start);

	printString("stack start: ", 13);
	print64Hex((uint64_t)stack_start);
	printNewLine();

	rsp =  (void *) ( (uint64_t * )stack_start + STACK_SIZE + 1);

	printString("rsp b: ", 7);
	print64Hex((uint64_t)rsp);
	printNewLine();
	
	void * new_rsp = (void*)( (uint64_t *)rsp - 20 );

	void * argc = 0;
	void * argv = 0xABCD;

	_fillstack(rsp, rip, argc, argv);

	addPCB(new_rsp , priority, stack_start);


	// for(int i = 0; i < 30; i++){
	// 	printString("when i= ", 8);
	// 	printDec(i);
	// 	printString(" rsp = ", 7);
	// 	print64Hex( (uint64_t *)rsp - i );
	// 	printString(" has: ", 6);
	// 	print64Hex( * ( (uint64_t *)rsp - i ) );
	// 	printNewLine();
	// }
	
	printString("new_rsp: ", 9);
	print64Hex( (uint64_t)new_rsp );
	printNewLine();

	printString("rip: ", 5);
	print64Hex((uint64_t)rip);
	printNewLine();
    printString("priority: ", 10);
	printDec(priority);
	printNewLine();
}




// | SS	| -> 0x0
// | RSP	| -> BP
// | RFLAGS| -> 0x202
// | CS	| -> 0x8
// | RIP	| -> main
// | Regs	| -> 0
// | ...	| -> (rdi) argc
// | uso	| -> 0
// | ...	| -> (rsi) argv
// | gene	| -> 0

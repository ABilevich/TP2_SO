/* sampleCodeModule.c */
#include <lib_user.h>
#include <shell.h>

void _sti_and_halt(void);

int main() {
	openPipe("stdpipe");
	//startShell();
	void (*shell)(void);
    shell = &startShell;
 	createProcess(shell, 2, 1, "shell",0,0);
	_sti_and_halt();
	
	return 0;
}
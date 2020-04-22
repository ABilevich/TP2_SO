/* sampleCodeModule.c */

#include <lib_user.h>
#include <shell.h>

int main() {
	
	//startShell();
	void (*shell)(void);
    shell = &startShell;
 	createProcess(shell, 27);
	
	return 0;
}
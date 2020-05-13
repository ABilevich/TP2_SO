/* sampleCodeModule.c */
#include <lib_user.h>
#include <shell.h>

void _sti_and_halt(void);

int main()
{
	pipe_info *pipe = openPipe("stdpipe");

	void (*shell)(void);
	shell = &startShell;
	createProcess(shell, 1, 1, "shell", pipe->id, pipe->id);
	_sti_and_halt();

	return 0;
}
#include <exceptions.h>
#include <naiveConsole.h>
#include <lib.h>
void _cli();
#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

typedef int (*EntryPoint)();

static void *const sampleCodeModuleAddress = (void *)0x400000;

static void jump(EntryPoint e);

void exceptionDispatcher(int exception, uint64_t *rsp)
{
	switch (exception)
	{

	case ZERO_EXCEPTION_ID:
		zero_division();
		break;

	case INVALID_OP_CODE:
		invalid_op_code();
		break;
	}

	reg_info(rsp);

	_outportb(0x20, 0x20);
	_sti();
	jump((EntryPoint)sampleCodeModuleAddress);
}

static void jump(EntryPoint e)
{
	ncPrintHex(e());
}

static void zero_division()
{
	exceptionMessage("ZERO DIVISION EXCEPTION\n\n", 26);
}

static void invalid_op_code()
{
	exceptionMessage("INVALID OPERATION CODE EXCEPTION\n\n", 35);
}
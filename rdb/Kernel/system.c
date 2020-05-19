#include <system.h>
#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>

void _sti_and_halt();

static void getMemory(uint64_t *pos, uint64_t *mem_buffer, unsigned int dim)
{
	for (int i = 0; i < dim; i++)
		mem_buffer[i] = pos[i];
}

int sys_system(void *option, void *arg1, void *arg2, void *arg3)
{
	switch ((uint64_t)option)
	{
	case 0:
		getMemory((uint64_t *)arg1, (uint64_t *)arg2, (uint64_t)arg3);
		break;
	case 1:
		malloc((size_t)arg1, (void **)arg2);
		break;
	case 2:
		free((void *)arg1);
		break;
	case 3:
		getFreeHeapSize((size_t *)arg1);
		break;
	case 4:
		getTotalHeapSize((size_t *)arg1);
		break;
	case 5:
		getTakenHeapSize((size_t *)arg1);
		break;
	case 6:
		_sti_and_halt();
		break;
	}
	return 0;
}
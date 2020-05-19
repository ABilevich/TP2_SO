#include <test_prio.h>

uint64_t my_getpid()
{
  return getPid();
}

uint64_t prio_create_process(char *name)
{
  void (*loop)(void);
  loop = &prio_endless_loop;
  return createProcess(loop, 1, 0, name, 0, 0);
}

uint64_t my_nice(uint64_t pid, uint64_t newPrio)
{
  return changeProcessPriority(pid, newPrio);
}

uint64_t my_prio_kill(uint64_t pid)
{
  return kill(pid);
}

uint32_t my_prio_block(uint32_t pid)
{
  return changeProcessState(pid, BLOCKED);
}

uint32_t my_prio_unblock(uint32_t pid)
{
  return changeProcessState(pid, READY);
}

void bussy_wait(uint64_t n)
{
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void prio_endless_loop()
{
  uint64_t pid = my_getpid();

  while (1)
  {
    printf("%d ", pid);
    bussy_wait(MINOR_WAIT);
  }
}

void test_prio()
{
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = prio_create_process("endless_loop");

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    switch (i % 3)
    {
    case 0:
      my_nice(pids[i], 1); //lowest priority
      break;
    case 1:
      my_nice(pids[i], 2); //medium priority
      break;
    case 2:
      my_nice(pids[i], 3); //highest priority
      break;
    }
  }

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_prio_block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    switch (i % 3)
    {
    case 0:
      my_nice(pids[i], 1); //medium priority
      break;
    case 1:
      my_nice(pids[i], 1); //medium priority
      break;
    case 2:
      my_nice(pids[i], 1); //medium priority
      break;
    }
  }

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_prio_unblock(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_prio_kill(pids[i]);
}
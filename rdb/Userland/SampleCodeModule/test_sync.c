#include <test_sync_async.h>

uint64_t sync_create_process(char *name)
{
  void (*func)(void);

  if (strcmp("my_process_inc", name) == 0)
  {
    func = &my_process_inc;
  }
  else if (strcmp("my_process_dec", name) == 0)
  {
    func = &my_process_dec;
  }
  else if (strcmp("my_process_inc_no_sem", name) == 0)
  {
    func = &my_process_inc_no_sem;
  }
  else if (strcmp("my_process_dec_no_sem", name) == 0)
  {
    func = &my_process_dec_no_sem;
  }

  return (uint64_t)createProcess(func, 1, 0, name, 0, 0);
}

sem_info *my_sem_open(char *name, uint64_t initialValue)
{
  return semOpen(name, initialValue);
}

uint64_t my_sem_wait(sem_info *sem)
{
  return semWait(sem);
}

uint64_t my_sem_post(sem_info *sem)
{
  return semPost(sem);
}

uint64_t my_sem_close(sem_info *sem)
{
  return semClose(sem);
}

uint64_t global; //shared memory

void slowInc(uint64_t *p, uint64_t inc)
{

  uint64_t aux = *p;
  for (int i = 0; i < 100; i++)
  {
  }
  aux += inc;
  for (int i = 0; i < 100; i++)
  {
  }
  *p = aux;
}

void my_process_inc()
{
  uint64_t i;

  sem_info *s = my_sem_open(SEM_ID, 1);
  if (s == NULL)
  {
    printf("ERROR OPENING SEM\n");
    return;
  }

  for (i = 0; i < N; i++)
  {
    my_sem_wait(s);
    slowInc(&global, 1);
    my_sem_post(s);
  }

  my_sem_close(s);

  printf("Final sync value: %d\n", global);
}

void my_process_dec()
{
  uint64_t i;

  sem_info *s = my_sem_open(SEM_ID, 1);
  if (s == NULL)
  {
    printf("ERROR OPENING SEM\n");
    return;
  }

  for (i = 0; i < N; i++)
  {
    my_sem_wait(s);
    slowInc(&global, -1);
    my_sem_post(s);
  }

  my_sem_close(s);

  printf("Final sync value: %d\n", global);
}

void test_sync()
{
  uint64_t i;
  global = 0;

  printf("CREATING PROCESSES...\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    sync_create_process("my_process_inc");
    sync_create_process("my_process_dec");
  }

  // The last one should print 0
}

void my_process_inc_no_sem()
{
  uint64_t i;
  for (i = 0; i < N; i++)
  {
    slowInc(&global, 1);
  }

  printf("Final async value: %d\n", global);
}

void my_process_dec_no_sem()
{
  uint64_t i;
  for (i = 0; i < N; i++)
  {
    slowInc(&global, -1);
  }

  printf("Final async value: %d\n", global);
}

void test_no_sync()
{
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    sync_create_process("my_process_inc_no_sem");
    sync_create_process("my_process_dec_no_sem");
  }

  // The last one should not print 0
}

void test_sync_async()
{
  test_sync();
  test_no_sync();
}

void run_test_sync()
{
  test_sync();
}

void run_test_async()
{
  test_no_sync();
}
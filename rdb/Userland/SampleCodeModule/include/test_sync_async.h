#ifndef _TEST_SYNC_ASYNC_H
#define _TEST_SYNC_ASYNC_H
#include <stdint.h>
#include <lib_user.h>

uint64_t sync_create_process(char *name);
sem_info *my_sem_open(char *sem_id, uint64_t initialValue);
uint64_t my_sem_wait(sem_info *sem);
uint64_t my_sem_post(sem_info *sem);
uint64_t my_sem_close(sem_info *sem);

#define N 100000
#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

void slowInc(uint64_t *p, uint64_t inc);
void my_process_inc();
void my_process_dec();
void test_sync();

void my_process_inc_no_sem();
void my_process_dec_no_sem();
void test_no_sync();
void test_sync_async();

void run_test_sync();
void run_test_async();

#endif
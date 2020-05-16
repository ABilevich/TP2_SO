#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H
#include <stdint.h>
#include <lib_user.h>

void test_mm();
uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void t_memset(void *start, uint8_t value, uint32_t size);

void endless_loop();
uint32_t my_create_process(char *name);
uint32_t my_kill(uint32_t pid);
uint32_t my_block(uint32_t pid);
uint32_t my_unblock(uint32_t pid);
void test_processes();
#endif
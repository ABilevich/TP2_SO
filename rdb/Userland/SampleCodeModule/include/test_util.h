#include <stdint.h>
#include <stdint.h>
#include <lib_user.h>

void test_mm();
uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void t_memset(void *start, uint8_t value, uint32_t size);
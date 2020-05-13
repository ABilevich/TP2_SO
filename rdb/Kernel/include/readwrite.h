#ifndef _READWRITE_H
#define _READWRITE_H
#include <stdint.h>
#include <keyboard.h>

int sys_read_write(void *option, void *arg1, void *arg2, void *arg3, void *arg4);
void rw_read(void *c, uint64_t input_id, uint64_t *resp);
void rw_write(char *c, uint64_t output_id, uint64_t *resp);
#endif
#ifndef _READWRITE_H
#define _READWRITE_H
#include <stdint.h>
#include <keyboard.h>

int sys_read_write(void * option, void * arg1, void * arg2, void * arg3, void * arg4);
int rw_read(void * c, uint64_t input_id, uint64_t * resp);
void rw_write(void * c, uint64_t input_id, uint64_t * resp);
#endif
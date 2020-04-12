#include <stdint.h>
#include <stddef.h>

void malloc(int wanted_size, void * ret_val);

void free( void * p);

void getFreeHeapSize(int * resp);


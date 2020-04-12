#include <stdint.h>
#include <stddef.h>

void malloc(size_t wanted_size, void ** ret_val);

void free( void * p);

void getFreeHeapSize(size_t * resp);


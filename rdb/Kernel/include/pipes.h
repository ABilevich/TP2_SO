#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>

#define PIPE_SIZE 256 

typedef struct pipe{
    uint64_t id;
    char * name;
    char is_open;
    char buff[PIPE_SIZE];
    uint64_t buff_taken_size;
} pipe;

typedef struct pipe_node{
    pipe * pipe;
    uint64_t proc_count;
    struct pipe_node * next;
    struct pipe_node * prev;
} pipe_node;

int sys_pipe(void * option, void * arg1, void * arg2, void * arg3, void * arg4);
void p_openPipe(char * name, uint64_t * resp_pipe_id);
pipe_node * p_createPipe(char * name, pipe_node * prev, pipe_node * next);
void p_closePipe(uint64_t id, uint64_t * resp);
int p_deletePipe(uint64_t id);
pipe * p_getPipe(uint64_t id);

int strcmp2(const char *s1, const char *s2);



#ifndef _PIPES_H
#define _PIPES_H

#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>
#include <screen.h>
#include <semaphore.h>

#define PIPE_SIZE 256

typedef struct p_prc_node
{
    uint64_t pid;
    struct p_prc_node *next;
} p_prc_node;

typedef struct pipe
{
    uint64_t id;
    char *name;
    char buff[PIPE_SIZE];
    uint64_t buff_taken_size;
    uint64_t r_sem_id;
    uint64_t w_sem_id;
    p_prc_node *procs;
} pipe;

typedef struct pipe_node
{
    pipe *pipe;
    struct pipe_node *next;
    struct pipe_node *prev;
} pipe_node;

int sys_pipe(void *option, void *arg1, void *arg2, void *arg3, void *arg4);
void p_openPipe(char *name, uint64_t pid, uint64_t *resp_pipe_id);
pipe_node *p_createPipe(char *name, pipe_node *prev, pipe_node *next);
void p_closePipe(uint64_t id, uint64_t pid, uint64_t *resp);
int p_deletePipe(uint64_t id);
pipe *p_getPipe(uint64_t id);
void pipeInit(char *name);

// sem *p_getRSem(uint64_t id);
// sem *p_getWSem(uint64_t id);

void openPipeForProc(uint64_t id, uint64_t pid);

void addProcessToPipe(pipe *pipe, uint64_t pid);
void RemoveProcessFromPipe(pipe *pipe, uint64_t pid);

void pipePrintAll();
void pipePrint(pipe *p);
void pipePrintProcs(p_prc_node *n);

int strcmp2(const char *s1, const char *s2);

#endif
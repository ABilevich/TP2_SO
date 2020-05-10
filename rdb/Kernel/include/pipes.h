#ifndef _PIPES_H
#define _PIPES_H

#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>
#include <screen.h>

#define PIPE_SIZE 256

enum modes
{
    U = 0,
    R,
    W,
    BR,
    BW
};

typedef struct prc_node
{
    uint64_t pid;
    uint64_t mode;
    struct prc_node *next;
} prc_node;

typedef struct pipe
{
    uint64_t id;
    char *name;
    char buff[PIPE_SIZE];
    uint64_t buff_taken_size;
    uint64_t proc_count;
    prc_node *procs;
} pipe;

typedef struct pipe_node
{
    pipe *pipe;
    struct pipe_node *next;
    struct pipe_node *prev;
} pipe_node;

void p_openPipe(char *name, uint64_t *resp_pipe_id);
pipe_node *p_createPipe(char *name, pipe_node *prev, pipe_node *next);
void p_closePipe(uint64_t id, uint64_t *resp);
int p_deletePipe(uint64_t id);
pipe *p_getPipe(uint64_t id);
void pipeInit(char *name);

void addProcessToPipe(pipe *pipe, uint64_t pid, uint64_t mode);
void RemoveProcessFromPipe(pipe *pipe, uint64_t pid);

void pipePrintAll();
void pipePrint(pipe *p);

int strcmp2(const char *s1, const char *s2);

#endif
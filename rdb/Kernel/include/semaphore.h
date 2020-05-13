#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <stdint.h>
#include <stddef.h>
#include <mm_interface.h>
#include <screen.h>
#include <processes.h>

typedef struct prc_node
{
    uint64_t pid;
    uint64_t is_blocked;
    struct prc_node *next;
} prc_node;

typedef struct sem
{
    void *name;
    uint64_t id;
    uint64_t cont;
    uint64_t lock;
    prc_node *procs;
} sem;

typedef struct sem_node
{
    sem *semaphore;
    struct sem_node *next;
} sem_node;

int sys_semaphore(void *option, void *arg1, void *arg2, void *arg3, void *arg4);

void s_semOpen(char *name, uint64_t pid, uint64_t start_cont, uint64_t *resp_id);
uint64_t createEmptySem(uint64_t start_cont);

void addProcessToSem(sem *sem, uint64_t pid);
void RemoveProcessFromSem(sem *sem, uint64_t pid);
void addProcessToSemViaId(uint64_t id, uint64_t pid);
void RemoveProcessFromSemViaId(uint64_t id, uint64_t pid);

sem_node *semCreate(char *name, uint64_t pid, uint64_t start_cont);
void s_semClose(uint64_t id, uint64_t pid, uint64_t *resp);
void s_semUnlink(char *name, uint64_t *resp);
void s_semWait(uint64_t id, uint64_t pid, uint64_t *resp);
void s_semPost(uint64_t id, uint64_t *resp);
int strcmp(const char *s1, const char *s2);

void semPrintAll();
void semPrint(sem *s);
void semPrintProcs(prc_node *n);

void spin_unlock(uint64_t *lock);
void spin_lock(uint64_t *lock);

sem *getSem(uint64_t id);
prc_node *getProc(sem *sem, uint64_t pid);

void unlockFirstBlockedProc(sem *semaphore);

void _irq00HandlerFake();
void _sti_and_halt();
void _forceInt20();
void _cli();
void _sti();

#endif
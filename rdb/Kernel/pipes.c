#include <pipes.h>

uint64_t next_pipe_id = 0;
pipe_node *current = NULL;
uint64_t total_pipes = 0;

int sys_pipe(void *option, void *arg1, void *arg2, void *arg3, void *arg4)
{
    switch ((uint64_t)option)
    {
    case 0:
        p_openPipe((char *)arg1, (uint64_t)arg2, (uint64_t *)arg3);
        break;
    case 1:
        p_closePipe((uint64_t)arg1, (uint64_t)arg2, (uint64_t *)arg3);
        break;
    case 2:
        pipePrintAll();
        break;
    }
    return 0;
}

void p_openPipe(char *name, uint64_t pid, uint64_t *resp_pipe_id)
{
    if (current == NULL)
    {
        pipeInit(name);
        addProcessToPipe(current->pipe, pid);
        *resp_pipe_id = current->pipe->id;
        return;
    }
    pipe_node *iterator = current;
    int i = 0;
    while (i < total_pipes)
    {
        if (name != NULL && strcmp2(iterator->pipe->name, name) == 0)
        {
            *resp_pipe_id = iterator->pipe->id;
            addProcessToPipe(iterator->pipe, pid);
            return;
        }
        i++;
        iterator = iterator->next;
    }
    pipe_node *aux = p_createPipe(name, iterator->prev, iterator->next);
    addProcessToPipe(aux->pipe, pid);
    *resp_pipe_id = aux->pipe->id;
}

void openPipeForProc(uint64_t id, uint64_t pid)
{
    pipe_node *iterator = current;
    int i = 0;
    while (i < total_pipes)
    {
        if (iterator->pipe->id == id)
        {
            addProcessToPipe(iterator->pipe, pid);
            return;
        }
        i++;
        iterator = iterator->next;
    }
}

pipe_node *p_createPipe(char *name, pipe_node *prev, pipe_node *next)
{
    pipe *p;
    malloc(sizeof(pipe), (void **)&p);
    p->name = name;
    p->id = next_pipe_id++;
    p->buff_taken_size = 0;
    p->procs = NULL;
    p->r_sem_id = createEmptySem(0);
    p->w_sem_id = createEmptySem(PIPE_SIZE);

    pipe_node *n;
    malloc(sizeof(pipe_node), (void **)&n);
    n->prev = prev;
    n->next = next;
    n->pipe = p;
    prev->next = n;
    next->prev = n;

    total_pipes++;

    return n;
}

void addProcessToPipe(pipe *pipe, uint64_t pid)
{
    p_prc_node *interator;
    p_prc_node *prev = NULL;
    p_prc_node *new;

    if (pipe->procs == NULL)
    {
        malloc(sizeof(p_prc_node), (void **)&new);
        new->pid = pid;
        new->next = NULL;
        pipe->procs = new;
        return;
    }
    else
    {
        interator = pipe->procs;
        while (interator != NULL)
        {
            if (interator->pid == pid)
            {
                return;
            }
            prev = interator;
            interator = interator->next;
        }

        malloc(sizeof(p_prc_node), (void **)&new);
        new->pid = pid;
        new->next = NULL;
        prev->next = new;
    }
}

void RemoveProcessFromPipe(pipe *pipe, uint64_t pid)
{
    p_prc_node *prev = NULL;
    p_prc_node *iterator = pipe->procs;

    while (iterator != NULL)
    {
        if (iterator->pid == pid)
        {
            if (prev == NULL)
            {
                if (iterator->next != NULL)
                {
                    pipe->procs = iterator->next;
                }
                else
                {
                    pipe->procs = NULL;
                }
            }
            else
            {
                prev->next = iterator->next;
            }
            free(iterator);
            return;
        }
        prev = iterator;
        iterator = iterator->next;
    }
    return;
}

void p_closePipe(uint64_t id, uint64_t pid, uint64_t *resp)
{
    pipe_node *iterator = current;
    for (int i = 0; i < total_pipes; i++)
    {
        if (iterator->pipe->id == id)
        {
            RemoveProcessFromPipe(iterator->pipe, pid);
            if (iterator->pipe->procs == NULL)
            {
                uint64_t junk;
                s_semClose(iterator->pipe->r_sem_id, pid, &junk);
                s_semClose(iterator->pipe->w_sem_id, pid, &junk);
                p_deletePipe(id);
                *resp = 1; //el pipe fue borrado
            }
            *resp = 0; //el pipe no fue borrado
            return;
        }
        iterator = iterator->next;
    }
    *resp = 2;
    return;
}

int p_deletePipe(uint64_t id)
{
    pipe_node *iterator = current;

    for (int i = 0; i < total_pipes; i++)
    {
        if (iterator->pipe->id == id)
        {
            iterator->prev->next = iterator->next;
            iterator->next->prev = iterator->prev;
            free(iterator->pipe);
            free(iterator);
            total_pipes--;
            return 0;
        }
        iterator = iterator->next;
    }
    return 1;
}

pipe *p_getPipe(uint64_t id)
{
    pipe_node *iterator = current;
    for (int i = 0; i < total_pipes; i++)
    {
        if (iterator->pipe->id == id)
        {
            return iterator->pipe;
        }
        iterator = iterator->next;
    }
    return NULL;
}

void pipeInit(char *name)
{
    current = p_createPipe(name, NULL, NULL);
    current->next = current;
    current->prev = current;
}

int strcmp2(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    while ((c1 = *s1++) == (c2 = *s2++))
    {
        if (c1 == '\0')
            return 0;
    }
    return c1 - c2;
}

void pipePrintAll()
{
    if (current == NULL)
    {
        printString("no pipes", 13);
        printNewLine();
        return;
    }
    printString("/--------------------------PIPES------------------------/", 57);
    printNewLine();
    pipe_node *iterator = current;
    for (int i = 0; i < total_pipes; i++)
    {
        pipePrint(iterator->pipe);
        iterator = iterator->next;
    }
    printString("/-------------------------------------------------------/", 57);
    printNewLine();
}

void pipePrint(pipe *p)
{
    printString("name: ", 6);
    if (p->name != NULL)
    {
        printString(p->name, 20);
    }
    else
    {
        printString("NULL", 4);
    }
    printString("  |  ", 5);
    printString("id: ", 4);
    printDec(p->id);
    printString("  |  ", 5);
    printString("procs: ", 7);
    pipePrintProcs(p->procs);
    printString("  |  ", 5);
    printString("r_pipe_id: ", 11);
    printDec(p->r_sem_id);
    printString("  |  ", 5);
    printString("w_pipe_id: ", 11);
    printDec(p->w_sem_id);
    printNewLine();
    printString("buffer: ", 8);
    if (p->buff_taken_size != 0)
    {
        printString(p->buff, p->buff_taken_size);
    }
    else
    {
        printString("empty...", 8);
    }
    printNewLine();
    printString("Blocked Processes: ", 19);
    printBlockedProcs(p->r_sem_id, p->w_sem_id);
    printNewLine();
}

void pipePrintProcs(p_prc_node *n)
{
    if (n == NULL)
    {
        printString("NULL", 4);
    }
    while (n != NULL)
    {
        printDec(n->pid);
        printString(", ", 2);
        n = n->next;
    }
}

void printBlockedProcs(uint64_t r_sem_id, uint64_t w_sem_id)
{
    printString("reading: ", 9);
    printBlockedProcsInSem(r_sem_id);
    printString(" - writing: ", 12);
    printBlockedProcsInSem(w_sem_id);
}

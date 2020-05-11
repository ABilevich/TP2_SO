#include <semaphore.h>

sem_node *first;
int next_id = 0;

int sys_semaphore(void *option, void *arg1, void *arg2, void *arg3, void *arg4)
{
    switch ((uint64_t)option)
    {
    case 0:
        s_semOpen((char *)arg1, (uint64_t)arg2, (uint64_t)arg3, (uint64_t *)arg4);
        break;
    case 1:
        s_semClose((uint64_t)arg1, (uint64_t)arg2, (uint64_t *)arg3);
        break;
    case 2:
        s_semUnlink((char *)arg1, (uint64_t *)arg2);
        break;
    case 3:
        s_semWait((uint64_t)arg1, (uint64_t)arg2, (uint64_t *)arg3);
        break;
    case 4:
        s_semPost((uint64_t)arg1, (uint64_t *)arg2);
        break;
    case 5:
        semPrintAll();
        break;
    }
    return 0;
}

void s_semOpen(char *name, uint64_t pid, uint64_t start_cont, uint64_t *resp_id)
{
    printString("sem open: ", 10);
    printDec(pid);
    printNewLine();
    semPrintAll();
    //buscar el semaforo
    if (first == NULL)
    { //si es el primero
        first = semCreate(name, pid, start_cont);
        *resp_id = first->semaphore->id;
    }
    else
    {
        sem_node *iterator = first;
        sem_node *prev = NULL;
        while (iterator != NULL)
        {
            if (iterator->semaphore->name != NULL && strcmp(iterator->semaphore->name, name) == 0)
            {
                //si estalo retorna
                *resp_id = iterator->semaphore->id;
                addProcessToSem(iterator->semaphore, pid);
                semPrintAll();
                return;
            }
            prev = iterator;
            iterator = iterator->next;
        }
        //sino lo crea y lo retorna
        prev->next = semCreate(name, pid, start_cont);
        *resp_id = prev->next->semaphore->id;
    }
    semPrintAll();
    return;
}

void addProcessToSem(sem *sem, uint64_t pid)
{
    prc_node *interator;
    prc_node *prev = NULL;
    prc_node *new;

    if (sem->procs == NULL)
    {
        malloc(sizeof(prc_node), (void **)&new);
        new->pid = pid;
        new->is_blocked = 0;
        new->next = NULL;
        sem->procs = new;
        return;
    }
    else
    {
        interator = sem->procs;
        while (interator != NULL)
        {
            if (interator->pid == pid)
            {
                return;
            }
            prev = interator;
            interator = interator->next;
        }

        malloc(sizeof(prc_node), (void **)&new);
        new->pid = pid;
        new->is_blocked = 0;
        new->next = NULL;
        prev->next = new;
    }
}

void RemoveProcessFromSem(sem *sem, uint64_t pid)
{
    prc_node *prev = NULL;
    prc_node *iterator = sem->procs;

    while (iterator != NULL)
    {
        if (iterator->pid == pid)
        {
            if (prev == NULL)
            {
                if (iterator->next != NULL)
                {
                    sem->procs = iterator->next;
                }
                else
                {
                    sem->procs = NULL;
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

sem_node *semCreate(char *name, uint64_t pid, uint64_t start_cont)
{
    //toma el nombre y crea el semaforo

    sem *new_sem;
    malloc(sizeof(sem), (void **)&new_sem);

    new_sem->name = name;
    new_sem->id = next_id;
    new_sem->cont = start_cont;
    new_sem->lock = 0;
    new_sem->procs = NULL;

    addProcessToSem(new_sem, pid);

    sem_node *new_node;
    malloc(sizeof(sem_node), (void **)&new_node);

    sem_node *iterator = first;
    sem_node *prev = NULL;

    while (iterator != NULL)
    {
        prev = iterator;
        iterator = iterator->next;
    }

    if (first == NULL)
    {
        first = new_node;
    }
    else
    {
        prev->next = new_node;
    }

    new_node->semaphore = new_sem;
    new_node->next = NULL;

    next_id++;

    return new_node;
}

void s_semClose(uint64_t id, uint64_t pid, uint64_t *resp)
{
    semPrintAll();
    //toma el id
    //busca el semaforo con ese id

    sem_node *iterator = first;
    sem_node *prev = NULL;
    while (iterator != NULL)
    {
        if (iterator->semaphore->id == id)
        {
            //printString("asd0", 4);
            //si lo encuentra
            //saca al proceso de la lista de procesos que lo usan
            RemoveProcessFromSem(iterator->semaphore, pid);
            //printString("asd1", 4);
            if (/*iterator->semaphore->name == NULL &&*/ iterator->semaphore->procs == NULL)
            {
                //si ningun otro proceso lo usa y el nombre es NULL lo borra
                if (prev == NULL)
                {
                    //si el semaforo que voy a borrar es tambien el primero de la lista
                    first = iterator->next;
                }
                else
                {
                    prev->next = iterator->next;
                }
                free(iterator->semaphore);
                free(iterator);
                *resp = 2; //hay que hacer free a sem_info
                semPrintAll();
                return;
                //printString("asd2", 4);
            }
            *resp = 0; //se removio el pid
            semPrintAll();
            return;
        }
        prev = iterator;
        iterator = iterator->next;
    }
    *resp = 1; //no se encontro un semaforo con ese id
    semPrintAll();
    return;
}

void s_semUnlink(char *name, uint64_t *resp)
{
    semPrintAll();

    sem_node *iterator = first;
    sem_node *prev = NULL;
    //toma el nombre
    //busca el nombre del semaforo
    while (iterator != NULL)
    {
        if (iterator->semaphore->name != NULL && strcmp(iterator->semaphore->name, name) == 0)
        {

            //chequea si no tengo procesos en mi lista
            if (iterator->semaphore->procs != NULL)
            {
                //si tengo pone el nombre en null
                iterator->semaphore->name = NULL;
            }
            else
            {
                if (prev == NULL)
                { //Si se esta borrando el 1er elemento
                    first = iterator->next;
                }
                else
                {
                    prev->next = iterator->next;
                }
                free(iterator->semaphore);
                free(iterator);
            }
            *resp = 0; // todo bien.
            semPrintAll();
            return;
        }
        prev = iterator;
        iterator = iterator->next;
    }
    *resp = -1; // no se encontro un semaforo con ese nombre
    semPrintAll();
    return;
}

void s_semWait(uint64_t id, uint64_t pid, uint64_t *resp)
{
    //printString("sem wait: ", 10);
    //printDec( pid );
    //printNewLine();
    //semPrintAll();

    //se fijan si el cont de un semaforo es superior a 0
    //si no lo es el proceso que llamo se bloquea
    //si lo es se decrementa en 1 el cont

    // sem_wait(){
    // spinlock()
    // if(sem == 0)
    //     block process
    // else
    //     sem--
    //     retornar
    // spinunlock()
    // }

    sem *sem = getSem(id);
    if (sem == NULL)
    {
        printString("no sem found", 12);
        printNewLine();
        *resp = -1;
        return;
    }

    prc_node *prc = getProc(sem, pid);
    if (prc == NULL)
    {
        printString("no prc found", 12);
        printNewLine();
        *resp = -2;
        return;
    }

    spin_lock(&(sem->lock));
    if (sem->cont == 0)
    {
        prc->is_blocked = 1;
        changeState(pid, BLOCKED_BY_SEM);
        spin_unlock(&(sem->lock));
        //_sti_and_halt();
        _forceInt20();
    }
    sem->cont--;
    spin_unlock(&(sem->lock));

    *resp = 0;
    //semPrintAll();
    return;
}

sem *getSem(uint64_t id)
{
    sem_node *iterator = first;
    while (iterator != NULL)
    {
        if (iterator->semaphore->id == id)
        {
            return iterator->semaphore;
        }
        iterator = iterator->next;
    }
    return NULL;
}

prc_node *getProc(sem *sem, uint64_t pid)
{
    prc_node *prc_iterator = sem->procs;
    while (prc_iterator != NULL)
    {
        if (prc_iterator->pid == pid)
        {
            return prc_iterator;
        }
        prc_iterator = prc_iterator->next;
    }
    return NULL;
}

void s_semPost(uint64_t id, uint64_t *resp)
{
    //printString("sem post id: ", 13);
    //printDec( id );
    //printNewLine();
    //semPrintAll();
    //aumenta en uno el cont del semaforo

    sem *sem = getSem(id);

    if (sem == NULL)
    {
        printString("no sem found", 12);
        printNewLine();
        *resp = -1;
        semPrintAll();
        return;
    }

    sem->cont++;
    unlockFirstBlockedProc(sem);
    *resp = 0;
    //semPrintAll();
    return;
}

void unlockFirstBlockedProc(sem *semaphore)
{
    prc_node *iterator = semaphore->procs;
    while (iterator != NULL)
    {
        if (iterator->is_blocked == 1)
        {
            iterator->is_blocked = 0;
            changeState(iterator->pid, READY);
            //printString("READY: ", 7);
            //printDec( iterator->pid );
            //printNewLine();
            return;
        }
        iterator = iterator->next;
    }
}

int strcmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    while ((c1 = *s1++) == (c2 = *s2++))
    {
        if (c1 == '\0')
            return 0;
    }
    return c1 - c2;
}

void semPrintAll()
{

    printString("/------------------------SEMAPHORES---------------------/", 57);
    printNewLine();
    if (first == NULL)
    {
        printString("no semaphores", 13);
        printNewLine();
    }
    else
    {
        sem_node *iterator = first;
        while (iterator != NULL)
        {
            semPrint(iterator->semaphore);
            iterator = iterator->next;
        }
    }
    printString("/-------------------------------------------------------/", 57);
    printNewLine();
}

void semPrint(sem *s)
{
    printString("name: ", 6);
    if (s->name != NULL)
    {
        printString(s->name, 20);
    }
    else
    {
        printString("NULL", 4);
    }
    printString("  |  ", 5);
    printString("id: ", 4);
    printDec(s->id);
    printString("  |  ", 5);
    printString("cont: ", 6);
    printDec(s->cont);
    printString("  |  ", 5);
    printString("procs: ", 7);
    semPrintProcs(s->procs);
    printNewLine();
}

void semPrintProcs(prc_node *n)
{
    if (n == NULL)
    {
        printString("NULL", 4);
    }
    while (n != NULL)
    {
        printString("(", 1);
        printDec(n->pid);
        printString(",", 1);
        printDec(n->is_blocked);
        printString(") ", 2);
        n = n->next;
    }
}

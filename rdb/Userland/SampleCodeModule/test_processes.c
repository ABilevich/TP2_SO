#include <stdio.h>
#include <test_util.h>

//TO BE INCLUDED
void endless_loop()
{
    while (1)
        ;
}

uint32_t my_create_process(char *name)
{
    void (*loop)(void);
    loop = &endless_loop;
    return createProcess(loop, 1, 0, name, 0, 0);
}

uint32_t my_kill(uint32_t pid)
{
    return kill(pid);
}

uint32_t my_block(uint32_t pid)
{
    return changeProcessState(pid, BLOCKED);
}

uint32_t my_unblock(uint32_t pid)
{
    return changeProcessState(pid, READY);
}

#define MAX_PROCESSES 100 //Should be around 80% of the the processes handled by the kernel

enum State
{
    T_ERROR,
    T_RUNNING,
    T_BLOCKED,
    T_KILLED
};

typedef struct P_rq
{
    uint32_t pid;
    enum State state;
} p_rq;

void test_processes()
{
    p_rq p_rqs[MAX_PROCESSES];
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;

    while (1)
    {

        // Create MAX_PROCESSES processes
        for (rq = 0; rq < MAX_PROCESSES; rq++)
        {
            p_rqs[rq].pid = my_create_process("endless_loop"); // TODO: Port this call as required

            //printf("new process pid: %d\n", p_rqs[rq].pid);  se puede descomentar esto para ver como crea todo, sino el pid avanza a las chapas
            if (p_rqs[rq].pid == -1)
            {                                       // TODO: Port this as required
                printf("Error creating process\n"); // TODO: Port this as required
                return;
            }
            else
            {
                p_rqs[rq].state = T_RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {

            for (rq = 0; rq < MAX_PROCESSES; rq++)
            {
                action = GetUniform(2) % 2;

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == T_RUNNING || p_rqs[rq].state == T_BLOCKED)
                    {
                        if (my_kill(p_rqs[rq].pid) == -1)
                        {                                      // TODO: Port this as required
                            printf("Error killing process\n"); // TODO: Port this as required
                            return;
                        }
                        p_rqs[rq].state = T_KILLED;
                        alive--;
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == T_RUNNING)
                    {
                        if (my_block(p_rqs[rq].pid) == -1)
                        {                                       // TODO: Port this as required
                            printf("Error blocking process\n"); // TODO: Port this as required
                            return;
                        }
                        p_rqs[rq].state = T_BLOCKED;
                    }
                    break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < MAX_PROCESSES; rq++)
                if (p_rqs[rq].state == T_BLOCKED && GetUniform(2) % 2)
                {
                    if (my_unblock(p_rqs[rq].pid) == -1)
                    {                                         // TODO: Port this as required
                        printf("Error unblocking process\n"); // TODO: Port this as required
                        return;
                    }
                    p_rqs[rq].state = T_RUNNING;
                }
        }
    }
}

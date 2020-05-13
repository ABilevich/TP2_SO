#include <phylo.h>

void phylo()
{
    int time;
    s_phylo *me = getLastPhylo();
    while (1)
    {
        time = (GetUniform(1000) % 1000);
        me->s = THINKING;
        printPhylos();
        wait(time); //rand
        me->s = HUNGRY;
        printPhylos();
        // ver que palito es el de menor numero
        semOpen(me->left_chop->chop_name, 1);
        semOpen(me->right_chop->chop_name, 1);
        sem_info *small = getSmallest(me);
        sem_info *big = getHeightest(me);
        takeChops(small, big);
        me->s = EATING;
        printPhylos();
        time = (GetUniform(1000) % 1000);
        wait(time);
        leaveChops(small, big);
        semClose(me->left_chop->chop_id);
        semClose(me->right_chop->chop_id);
    }
}

void takeChops(sem_info *small, sem_info *big)
{
    semWait(small);
    semWait(big);
}
void leaveChops(sem_info *small, sem_info *big)
{
    semPost(small);
    semPost(big);
}

sem_info *getSmallest(s_phylo *me)
{
    return (me->left_chop->chop_id->id < me->right_chop->chop_id->id) ? me->left_chop->chop_id : me->right_chop->chop_id;
}

sem_info *getHeightest(s_phylo *me)
{
    return (me->left_chop->chop_id->id < me->right_chop->chop_id->id) ? me->right_chop->chop_id : me->left_chop->chop_id;
}

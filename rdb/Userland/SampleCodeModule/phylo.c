#include <phylo.h>

static void takeChops(sem_info *small, sem_info *big);
static void leaveChops(sem_info *small, sem_info *big);
static sem_info *getSmallest(s_phylo *me);
static sem_info *getHeightest(s_phylo *me);

void phylo()
{
    s_phylo *me = getLastPhylo();
    // while (1)
    // {
    //     me->s = THINKING;
    //     wait(2); //rand
    //     me->s = HUNGRY;
    //     // ver que palito es el de menor numero
    //     semOpen(me->left_chop->chop_name,1);
    //     semOpen(me->right_chop->chop_name,1);
    //     sem_info *  small = getSmallest(me);
    //     sem_info *  big = getHeightest(me);
    //     takeChops(small,big);
    //     me->s = EATING;
    //     wait(2);
    //     leaveChops(small,big);
    //     semClose(me->left_chop->chop_id);
    //     semClose(me->right_chop->chop_id);
    // }
}

static void takeChops(sem_info *small, sem_info *big)
{
    semWait(small);
    semWait(big);
}
static void leaveChops(sem_info *small, sem_info *big)
{
    semPost(small);
    semPost(big);
}

static sem_info *getSmallest(s_phylo *me)
{
    return (me->left_chop->chop_id->id < me->right_chop->chop_id->id) ? me->left_chop->chop_id : me->right_chop->chop_id;
}

static sem_info *getHeightest(s_phylo *me)
{
    return (me->left_chop->chop_id->id < me->right_chop->chop_id->id) ? me->right_chop->chop_id : me->left_chop->chop_id;
}

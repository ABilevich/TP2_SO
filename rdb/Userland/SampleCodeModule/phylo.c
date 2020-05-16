#include <phylo.h>

void phylo()
{
    s_phylo *me = getLastPhylo();

    semOpen(me->my_sem_name,1);  //abro mi semaforo una vez
    while (me->i_love_philosofating)
    {
        
        me->s = THINKING;
        printPhylos();
        waitRand(); //pienso

        me->s = HUNGRY;
        printPhylos();

        semWait(me->my_sem); // me fijo si puedo entrar (solo no puedo cuando se esta agregando un phylo)
        
        if(me->i_love_philosofating){
            semOpen(me->left_chop->chop_name, 1); // ver que palito es el de menor numero
            semOpen(me->right_chop->chop_name, 1);
            sem_info *small = getSmallest(me);
            sem_info *big = getHeightest(me);
            takeChops(small, big); //hago wait en los palitos
            me->s = EATING; 
            printPhylos();
            waitRand(); //como
            leaveChops(small, big); // hago post en los palitos
            semClose(me->left_chop->chop_id);
            semClose(me->right_chop->chop_id);
        }

        semPost(me->my_sem);
    }
    semClose(me->my_sem);
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

void waitRand(){
    wait((GetUniform(1000) % 1000)); //pienso
    return;
}



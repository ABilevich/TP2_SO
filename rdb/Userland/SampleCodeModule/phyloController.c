#include <phyloController.h>

s_node *first = NULL;

uint64_t phylo_counter = 0;
uint64_t chop_counter = 0;
char started = 0;
s_phylo *last_phylo = NULL;

void startPhyloController()
{
    for (size_t i = 0; i < INITIAL_PHYLO_CANT; i++)
    {
        addPhylo();
    }
    started = 1;
    while (1)
    {
        char c = 0;
        if ((c = scanChar()) != 0)
        {
            if (c == 'a')
            {
                addPhylo();
            }
            else if (c == 'r')
            {
                removePhylo();
            }
        }
    }
}

void addPhylo()
{
    if (started == 0)
    {
        printf("Phylos isn't started, use de phylo command to start\n");
        return;
    }
    printf("adding Pyloh...\n");

    s_phylo *new_phylo = createPhylo();

    s_node *aux = malloc(sizeof(s_node));
    aux->ph = new_phylo;

    if (phylo_counter == 0)
    {
        new_phylo->left_chop = createChop();

        new_phylo->right_chop = createChop();

        aux->next = aux;
        aux->prev = aux;
        first = aux;
    }
    else if (phylo_counter == 1)
    {
        first->next = aux;
        first->prev = aux;
        aux->next = first;
        aux->prev = first;
        aux->ph->left_chop = first->ph->right_chop;
        aux->ph->right_chop = first->ph->left_chop;
    }
    else
    {
        s_chopstick *new_chop = createChop();
        new_phylo->left_chop = new_chop;

        semWait(first->prev->ph->my_sem);
        semWait(first->ph->left_chop->chop_id);

        new_phylo->right_chop = first->ph->left_chop;
        first->prev->ph->right_chop = new_chop;

        first->prev->next = aux;
        aux->prev = first->prev;
        first->prev = aux;
        aux->next = first;

        semPost(first->ph->left_chop->chop_id);
        semPost(first->prev->prev->ph->my_sem);
    }
    last_phylo = new_phylo;
    phylo_counter++;

    void (*p)(void);
    p = &phylo;
    new_phylo->id = createProcess(p, 1, 0, "p", 0, 0);
}

s_phylo *createPhylo()
{
    s_phylo *new_phylo = malloc(sizeof(s_phylo));
    if (new_phylo == NULL)
    {
        printf("createPhylo ERROR\n");
    }
    new_phylo->id = phylo_counter;
    new_phylo->id = -1;
    new_phylo->i_love_philosofating = 1;
    new_phylo->s = THINKING;
    char *aux_p = "_pylo_sem";
    uintToBase(phylo_counter, new_phylo->my_sem_name, 10);
    my_strcat(new_phylo->my_sem_name, aux_p);
    new_phylo->my_sem = semOpen(new_phylo->my_sem_name, 1);
    return new_phylo;
}

s_chopstick *createChop()
{
    s_chopstick *new_chop = malloc(sizeof(s_chopstick));
    if (new_chop == NULL)
    {
        printf("createPhylo ERROR\n");
    }
    char *aux_c = "_chop_sem";
    uintToBase(chop_counter, new_chop->chop_name, 10);
    my_strcat(new_chop->chop_name, aux_c);
    sem_info *sem = semOpen(new_chop->chop_name, 1);
    new_chop->chop_id = sem;
    chop_counter++;
    return new_chop;
}

void removePhylo()
{
    if (started == 0)
    {
        printf("Phylos isn't started, use de phylo comand to start\n");
        return;
    }

    if (phylo_counter <= 0)
    {
        printf("cant have less than 0 phylos!\n");
        return;
    }

    printf("removing Pyloh...\n");

    phylo_counter--;

    if (phylo_counter == 0)
    {
        first->ph->i_love_philosofating = 0;
        semWait(first->ph->my_sem);
        semPost(first->ph->my_sem);

        semClose(first->ph->my_sem);
        semClose(first->ph->left_chop->chop_id);
        semClose(first->ph->right_chop->chop_id);
        free(first->ph->right_chop);
        free(first->ph->left_chop);
        free(first);
        chop_counter -= 2;
    }
    else if (phylo_counter == 1)
    {
        first->prev->ph->i_love_philosofating = 0;
        semWait(first->prev->ph->my_sem);
        semPost(first->prev->ph->my_sem);

        semClose(first->prev->ph->my_sem);
        free(first->prev);
    }
    else if (phylo_counter >= 2)
    {
        semWait(first->prev->prev->ph->my_sem);
        first->prev->ph->i_love_philosofating = 0;
        semWait(first->prev->ph->my_sem);
        semPost(first->prev->ph->my_sem);
        semClose(first->prev->ph->left_chop->chop_id);
        free(first->prev->ph->left_chop);

        s_node *aux_to_del = first->prev;
        first->prev = first->prev->prev;
        first->prev->next = first;

        first->prev->ph->right_chop = first->ph->left_chop;

        semClose(aux_to_del->ph->my_sem);
        free(aux_to_del->ph);
        free(aux_to_del);

        semPost(first->prev->ph->my_sem);

        chop_counter--;
    }
}

void printPhylos()
{
    if (first == NULL)
    {
        printf("No Phylos \n");
        return;
    }

    s_node *iterator = first;
    for (int i = 0; i < phylo_counter; i++, iterator = iterator->next)
    {
        switch (iterator->ph->s)
        {
        case EATING:
            printf("E");
            break;
        default:
            printf(".");
            break;
        }
    }
    printf("\n");
}

s_phylo *getLastPhylo()
{
    return last_phylo;
}
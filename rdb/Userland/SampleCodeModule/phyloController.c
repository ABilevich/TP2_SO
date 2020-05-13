#include <phyloController.h>

s_node *first = NULL;

uint64_t phylo_counter = 0;
uint64_t chop_counter = 0;

s_phylo *last_phylo = NULL;

void startPhyloController()
{
    for (size_t i = 0; i < INITIAL_PHYLO_CANT; i++)
    {
        addPhylo();
    }

    // addPhylo();
    // printPhylos();
    // addPhylo();
    // printPhylos();
    // while(1){
    //     char c = 0;
    //     if((c = scanChar()) != 0){
    //         if(c == 'a'){
    //             addPhylo();
    //         }
    //         else if(c == 'r'){
    //             removePhylo();
    //         }
    //     //wait(1);

    //     }
    //     //printf("Sergio: %d",first->ph->id);
    //     //printPhylos();
    //     //printf("\n");
    // }
}

void addPhylo()
{
    printf("adding Pyloh...\n");

    s_phylo *new_phylo = createPhylo();

    s_node *aux = malloc(sizeof(s_node));
    aux->ph = new_phylo;

    if (phylo_counter == 1)
    {
        new_phylo->left_chop = createChop();
        //printf("left chop: %s\n", new_phylo->left_chop->chop_name);

        new_phylo->right_chop = createChop();
        //printf("right chop: %s\n", new_phylo->right_chop->chop_name);

        aux->next = aux;
        aux->prev = aux;
        first = aux;
    }
    else if (phylo_counter == 2)
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
        new_phylo->left_chop = createChop();

        //falta agregar que si el ultimo filo que meti mientras hice
        //semWait, me pide su palito justo, cuando lo reasigne todo,
        //el va a seguir pidiendo el palito viejo y no el nuevo.
        //Deberia bloquear al ultimo directamente. Hay que discutirlo!

        semWait(first->ph->left_chop->chop_id);

        new_phylo->right_chop = first->ph->left_chop;
        first->prev->ph->right_chop = new_phylo->left_chop;

        first->prev->next = aux;
        aux->prev = first->prev;
        first->prev = aux;
        aux->next = first;

        semPost(first->ph->left_chop->chop_id);
    }

    last_phylo = new_phylo;

    //printPhylos();
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
    new_phylo->s = THINKING;
    phylo_counter++;
    return new_phylo;
}

s_chopstick *createChop()
{
    s_chopstick *new_chop = malloc(sizeof(s_chopstick));
    if (new_chop == NULL)
    {
        printf("createPhylo ERROR\n");
    }
    uintToBase(chop_counter, new_chop->chop_name, 10);
    //printf("%s\n", new_chop->chop_name);
    sem_info *sem = semOpen(new_chop->chop_name, 1);
    new_chop->chop_id = sem;
    chop_counter++;
    return new_chop;
}

void removePhylo()
{
    printf("removing Pyloh...\n");
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
        //printf("%s", iterator->ph->left_chop->chop_name);
        switch (iterator->ph->s)
        {
        case EATING:
            printf("E");
            break;
        default:
            printf(".");
            break;
        }
        // PrintAllSemInfo();
        //printf("%s", iterator->ph->right_chop->chop_name);
    }
    printf("\n");
}

s_phylo *getLastPhylo()
{
    return last_phylo;
}
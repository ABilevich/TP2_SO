#include <phyloController.h>

s_node *first = NULL;
uint64_t phylo_counter = 0;
uint64_t chop_counter = 0;

s_phylo *last_phylo = NULL;

void startPhyloController()
{
    addPhylo();
    printPhylos();
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

    s_phylo *new_phylo;
    new_phylo = malloc(sizeof(s_phylo));
    new_phylo->id = phylo_counter;
    new_phylo->s = THINKING;

    s_node *aux = malloc(sizeof(s_node));
    aux->ph = new_phylo;

    if (phylo_counter == 0)
    {

        char sem_name[10];
        uintToBase(chop_counter, sem_name, 10);
        printf("%s\n", sem_name);
        sem_info *sem = semOpen(sem_name, 1);
        chop_counter++;
        new_phylo->left_chop = malloc(sizeof(s_chopstick));
        new_phylo->left_chop->chop_id = sem;
        new_phylo->left_chop->chop_name = malloc(1 * sizeof(char));
        strcpy(new_phylo->left_chop->chop_name, sem_name);

        printf("left chop: %s\n", new_phylo->left_chop->chop_name);
        char sem_name2[10];
        uintToBase(chop_counter, sem_name2, 10);

        sem_info *sem2 = semOpen(sem_name2, 1);

        chop_counter++;
        new_phylo->right_chop = malloc(sizeof(s_chopstick));
        new_phylo->right_chop->chop_id = sem2;
        // new_phylo->right_chop->chop_name = sem_name2;
        new_phylo->left_chop->chop_name = malloc(strlen(sem_name2) * sizeof(char));
        strcpy(new_phylo->right_chop->chop_name, sem_name2);
        printf("right chop: %s\n", new_phylo->right_chop->chop_name);

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
        char sem_name[10];
        uintToBase(0, sem_name, 10);
        sem_info *sem = semOpen(sem_name, 1);
        chop_counter++;
        new_phylo->left_chop = malloc(sizeof(s_chopstick));
        new_phylo->left_chop->chop_id = sem;
        new_phylo->left_chop->chop_name = sem_name;

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

    phylo_counter++;
    last_phylo = aux->ph;

    printPhylos();
    // void (*p)(void);
    // p = &phylo;
    // createProcess(p, 1, 0, "p", 0, 0);
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
        switch (iterator->ph->s)
        {
        case EATING:
            printf("E");
            break;
        case THINKING:
            printf(".");
            break;
        case HUNGRY:
            printf("H");
            break;
        }
        printf("\n");
        // PrintAllSemInfo();
        // if(iterator->ph->left_chop->chop_name == NULL){
        //     printf("es null gaturro :/ \n");
        // }else{
        //     printf("left chop: %d aca tenes gato\n",iterator->ph->left_chop->chop_id->id);
        // }
        // printf("left chop: %d\n",iterator->ph->left_chop->chop_id->id);
        // printf("right chop: %d\n",iterator->ph->right_chop->chop_id->id);
        printf("left chop: %s\n", iterator->ph->left_chop->chop_name);
        printf("right chop: %s\n", iterator->ph->right_chop->chop_name);
    }
}

s_phylo *getLastPhylo()
{
    return last_phylo;
}
#ifndef _PHYLOCONTROLLER_H
#define _PHYLOCONTROLLER_H

#define MAX_CHOP_DIGIT 10
#define INITIAL_PHYLO_CANT 5

#include <lib_user.h>

void phylo();

typedef enum state
{
    THINKING = 0,
    HUNGRY,
    EATING
} state;

typedef struct s_chopstick
{
    sem_info *chop_id;
    char chop_name[MAX_CHOP_DIGIT];

} s_chopstick;

typedef struct s_phylo
{
    uint64_t id;
    uint64_t pid;
    state s;
    s_chopstick *left_chop;
    s_chopstick *right_chop;

} s_phylo;

typedef struct s_node
{
    s_phylo *ph;
    struct s_node *next;
    struct s_node *prev;
} s_node;

#include <phylo.h>

void startPhyloController();
void addPhylo();
void removePhylo();
void printPhylos();
s_phylo *getLastPhylo();

s_phylo *createPhylo();
s_chopstick *createChop();

#endif
#include <lib_user.h>

typedef enum state
{
    THINKING = 0,
    HUNGRY,
    EATING
} state;

typedef struct s_phylo
{
    uint64_t id;
    state s;
    sem_info *left_chop;
    sem_info *right_chop;
} s_phylo;

typedef struct s_node
{
    s_phylo *ph;
    struct s_node *next;
    struct s_node *prev;
} s_node;

void startPhyloController();
void addPhylo();
void removePhylo();
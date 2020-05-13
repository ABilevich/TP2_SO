#ifndef _PHYLO_H
#define _PHYLO_H
#include <phyloController.h>

void phylo();
void takeChops(sem_info *small, sem_info *big);
void leaveChops(sem_info *small, sem_info *big);
sem_info *getSmallest(s_phylo *me);
sem_info *getHeightest(s_phylo *me);

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);

#endif
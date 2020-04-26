#ifndef _CMDS_H
#define _CMDS_H

#include <aracnoid.h>
void startAracnoid(gameState * save_file, int * saved);
void printUserManual(void); 
void getLocalTime(void);
void printRegistersInfo(void);
void clear(void);
void printMemoryStatus(long int offset); 
void test(char * option);
void command_set(char * option, char * color);
void testDivException(void);
void testInvOpCode(void);

void p_kill(uint64_t pid);
void ps();
void run(void (*func)(void), char * name, char fg);
void nice(uint64_t pid, uint64_t priority);
void chstate(uint64_t pid, char state);

void testMem(void);
void testMem1(void);
void testMem2(void);
void testMem3(void);
void testMem4(void);
void testMem5(void);

void testProcess(void);

#endif
#include <test_sem.h>

#define SIZE 10

void test_sem()
{
    void (*func)(void);

    for (int i = 0; i < SIZE; i++)
    {
        func = &start_b;
        createProcess(func, 2, 0, "b", 0, 0);
        func = &start_c;
        createProcess(func, 2, 0, "c", 0, 0);
    }
}
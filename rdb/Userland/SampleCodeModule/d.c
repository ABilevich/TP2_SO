#include <d.h>
#include <e.h>

void start_d(void)
{

    void (*e1)(void);
    e1 = &start_e;
    createProcess(e1, 2, 0, "e", 0, 0);
}
#include <loop.h>

void start_loop(void)
{
    while (1)
    {
        wait(500);
        printf("Loop PID: %d\n", getPid());
    }
}
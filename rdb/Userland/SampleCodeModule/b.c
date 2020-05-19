#include <b.h>
#include <c.h>

void start_b(void)
{
    int *variable = (void *)0xF00000;
    int old;
    sem_info *si = semOpen("pepito", 1);
    int i = 0;
    while (i < 1000)
    {
        semWait(si);

        old = *variable;
        old++;
        *variable = old;

        i++;
        semPost(si);
    }

    semClose(si);

    printf("B final counter: %d\n", *variable);
}
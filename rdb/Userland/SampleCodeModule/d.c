#include <d.h>
#include <e.h>

void start_d(void)
{
    void (*e1)(void);
    e1 = &start_e;
    createProcess(e1, 1, 0, "e", 0, 0);

    wait(1);

    printAllProcessInfo();

    pipe_info *p = openPipe("test");

    char *message1 = "Aguante Po!!";
    int size1 = strlen(message1);
    char *message2 = "El gerrero dragon!!";
    int size2 = strlen(message2);

    write_to(message1, 12, p->id);
    write_to(message2, 20, p->id);

    printf("i wrote: %s\n", message1);
    printf("i wrote: %s\n", message2);

    // PrintAllPipeInfo();
    // PrintAllSemInfo();
    // printAllProcessInfo();
    // PrintAllPipeInfo();

    closePipe(p);

    // wait(1);
    // printAllProcessInfo();
    // PrintAllPipeInfo();
    // PrintAllSemInfo();
}
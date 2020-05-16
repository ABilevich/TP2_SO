#include <d.h>
#include <e.h>

void start_d(void)
{
    void (*e1)(void);
    e1 = &start_e;
    createProcess(e1, 1, 0, "e", 0, 0);

    wait(1000);

    //printAllProcessInfo();

    pipe_info *p = openPipe("test");

    char *message1 = "Aguante Po!!";
    //int size1 = strlen(message1);
    char *message2 = "El guerrero dragon!!";
    //int size2 = strlen(message2);
    char *message3 = "123456789";
    //int size3 = strlen(message3);

    write_to(message1, 12, p->id);
    //PrintAllPipeInfo();
    write_to(message2, 20, p->id);
    //PrintAllPipeInfo();
    write_to(message3, 9, p->id);
    //PrintAllPipeInfo();

    printf("i wrote: %s\n", message1);
    printf("i wrote: %s\n", message2);
    printf("i wrote: %s\n", message3);

    
    //PrintAllSemInfo();
    // printAllProcessInfo();
    // PrintAllPipeInfo();

    closePipe(p);

    // printAllProcessInfo();
    //PrintAllPipeInfo();
    //PrintAllSemInfo();
}
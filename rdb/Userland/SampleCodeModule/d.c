#include <d.h>
#include <e.h>

void start_d(void)
{
    void (*e1)(void);
    e1 = &start_e;
    createProcess(e1, 1, 0, "e", 0, 0);

    wait(1000);

    pipe_info *p = openPipe("test");

    char *message1 = "Aguante Po!!";
    char *message2 = "El guerrero dragon!!";
    char *message3 = "123456789";

    write_to(message1, 100, p->id);
    write_to(message2, 100, p->id);
    write_to(message3, 100, p->id);

    printf("i wrote: %s\n", message1);
    printf("i wrote: %s\n", message2);
    printf("i wrote: %s\n", message3);

    closePipe(p);
}
#include <e.h>

void start_e(void)
{

    pipe_info *p = openPipe("test");

    char *message;

    read_from(message, 33, p->id);

    printf("i read: %s\n", message);

    // // PrintAllPipeInfo();
    // // PrintAllSemInfo();

    closePipe(p);

    // // PrintAllPipeInfo();
    // // PrintAllSemInfo();
    // printf("byeee\n");
}
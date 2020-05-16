#include <e.h>

void start_e(void)
{

    pipe_info *p = openPipe("test");

    char *message = malloc(100 * sizeof(char));

    read_from(message,35 , p->id);
    //PrintAllPipeInfo();

    printf("i read: %s\n", message);

    // PrintAllPipeInfo();
    // PrintAllSemInfo();

    closePipe(p);

    // PrintAllPipeInfo();
    // PrintAllSemInfo();
    printf("byeee\n");
}
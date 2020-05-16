#include <e.h>

void start_e(void)
{

    pipe_info *p = openPipe("test");

    char *message = malloc(100 * sizeof(char));

    scan_from(message,100, p->id);
    printf("i read: %s\n", message);
    scan_from(message,100, p->id);
    printf("i read: %s\n", message);
    scan_from(message,100, p->id);
    printf("i read: %s\n", message);

    wait(1000);
    printf("--------------\n", message);
    
    write("hola buendia test xdxdxdx", 1000);

    printf("--------------\n", message);

    // PrintAllPipeInfo();
    // PrintAllSemInfo();

    closePipe(p);

    // PrintAllPipeInfo();
    // PrintAllSemInfo();
    printf("byeee\n");
}
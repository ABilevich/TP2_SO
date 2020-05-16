#include <cat.h>

void start_cat(){
    char * c = malloc(1);
    while(read(c, 1) != 0){
        write(c,1);
    }
}
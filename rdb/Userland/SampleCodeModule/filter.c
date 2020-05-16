#include <filter.h>

void start_filter(){

    char * c = malloc(1);

    while(read(c, 1) != 0){
        if( *c != 'a' &&  *c != 'e' &&  *c != 'i' &&  *c != 'o' &&  *c != 'u'){
            write(c,1);   
        }
    }

}
#include <pipes.h>


uint64_t next_pipe_id = 0;
pipe_node * current = NULL;
uint64_t total_pipes = 0;


int sys_pipe(void * option, void * arg1, void * arg2, void * arg3, void * arg4) {
    switch ((uint64_t) option) {
	case 0:
		p_openPipe((char *) arg1, (uint64_t *)arg2);
		break;
	case 1:
		p_closePipe((uint64_t) arg1, (uint64_t *)arg2);
		break;
	}
	return 0;
}


void p_openPipe(char * name, uint64_t * resp_pipe_id){
    if(current == NULL){
        pipeInit(name);
        pipePrintAll();
        return;
    }
    pipe_node * iterator = current;
    int i = 0;
    while(i < total_pipes){
        if(name != NULL && strcmp2(iterator->pipe->name,name) == 0){
            *resp_pipe_id = iterator->pipe->id; 
            iterator->pipe->proc_count++;
            pipePrintAll();
            return;
        }
        i++;
        iterator = iterator->next;
    }
    pipe_node * aux = p_createPipe(name,iterator->prev,iterator->next);
    aux->pipe->proc_count++;
    *resp_pipe_id = aux->pipe->id;   
    pipePrintAll();
}

pipe_node * p_createPipe(char * name, pipe_node * prev, pipe_node * next){
    
    pipe * p;
    malloc(sizeof(pipe), (void **)&p);
    p->name = name;
    p->id = next_pipe_id++;
    p->buff_taken_size = 0;
    p->proc_count = 0;

    pipe_node * n;
    malloc(sizeof(pipe_node), (void **)&n);
    n->prev = prev;
    n->next = next;
    n->pipe = p;
    prev->next = n;
    next->prev = n;

    total_pipes++;

    return n;
}

void p_closePipe(uint64_t id, uint64_t * resp){
    if(id == 0){
        *resp = 2; 
        return;
    }
    pipe_node * iterator = current;
    for(int i = 0 ; i < total_pipes; i++){
        if(iterator->pipe->id == id){
            iterator->pipe->proc_count--;
            if(iterator->pipe->proc_count == 0){
                p_deletePipe(id);
            }
            *resp = 0;
            return;
        }
    }
    *resp = 1;
    return;
}

int p_deletePipe(uint64_t id){
    pipe_node * iterator = current;

    for(int i = 0 ; i < total_pipes; i++){
        if(iterator->pipe->id == id){
            iterator->prev->next = iterator->next;
            iterator->next->prev = iterator->prev;
            free(iterator->pipe);
            free(iterator); 
            total_pipes--;
            return 0;
        }
    }
    return 1;
}

pipe * p_getPipe(uint64_t id){
    pipe_node * iterator = current;
    for(int i = 0 ; i < total_pipes; i++){
        if(iterator->pipe->id == id){
            return iterator->pipe;
        }
    }
    return NULL;
}

void pipeInit(char * name){
    current = p_createPipe(name, NULL, NULL);
    current->next = current;
    current->prev = current;
    current->pipe->proc_count++;
}

int strcmp2(const char *s1, const char *s2) {
    unsigned char c1, c2;
    while ((c1 = *s1++) == (c2 = *s2++)) {
        if (c1 == '\0')
            return 0;
    }
    return c1 - c2;
}

void pipePrintAll(){
    if(current == NULL){
        printString("no pipes", 13);
        printNewLine();
        return;
    }
    printString("/--------------------------PIPES------------------------/", 57);
    printNewLine();
    pipe_node * iterator = current;
    for(int i = 0 ; i < total_pipes; i++){
        pipePrint(iterator->pipe);
        iterator = iterator->next;
    }
    printString("/-------------------------------------------------------/", 57);
    printNewLine();
}

void pipePrint(pipe* p){
    printString("name: ", 6);
    if(p->name != NULL){
        printString(p->name, 20);
    }else{
        printString("NULL", 4);
    }
    printString("  |  ", 5);
    printString("id: ", 4);
	printDec( p->id );
    printString("  |  ", 5);
    printString("cont: ", 6);
	printDec( p->proc_count );
    printNewLine();
}
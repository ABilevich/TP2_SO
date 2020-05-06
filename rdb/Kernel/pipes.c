#include <pipes.h>


uint64_t next_pipe_id = 1;
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
        current = p_createPipe(name, NULL, NULL);
        current->next = current;
        current->prev = current;
        current->proc_count++;
        total_pipes++;
        return;
    }else{
        pipe_node * iterator = current;
        int i = 0;
        while(i < total_pipes){
            if(strcmp2(iterator->pipe->name,name) == 0){
                *resp_pipe_id = iterator->pipe->id; 
                iterator->proc_count++;
                return;
            }
            i++;
            iterator = iterator->next;
        }
        pipe_node * aux = p_createPipe(name,iterator->prev,iterator->next);
        aux->proc_count++;
        *resp_pipe_id = aux->pipe->id;
    }
    
}

pipe_node * p_createPipe(char * name, pipe_node * prev, pipe_node * next){
    
    pipe * p;
    malloc(sizeof(pipe), (void **)&p);

    p->name = name;
    p->id = next_pipe_id++;
    p->is_open = 1;
    p->buff_taken_size = 0;


    pipe_node * n;
    malloc(sizeof(pipe_node), (void **)&n);
    n->prev = prev;
    n->next = next;
    n->pipe = p;
    n->proc_count = 0;
    prev->next = n;
    next->prev = n;

    return n;

}

void p_closePipe(uint64_t id, uint64_t * resp){
    pipe_node * iterator = current;
    for(int i = 0 ; i < total_pipes; i++){
        if(iterator->pipe->id == id){
            iterator->proc_count--;
            
            if(iterator->proc_count == 0){
                p_deletePipe(id);
                *resp = 0;
                return;
            }
            if(iterator->pipe->is_open){
                iterator->pipe->is_open = 0;
            }
            *resp = 0;
            return;
        }
    }
    *resp = -1;
    return;
}

int p_deletePipe(uint64_t id){
    pipe_node * iterator = current;

    if(total_pipes == 0){
        return -1;
    }

    if(total_pipes == 1){
        free(iterator->pipe);
        free(iterator);
        current = NULL;
        return 0;
    }

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
    return -1;
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

int strcmp2(const char *s1, const char *s2) {
    unsigned char c1, c2;
    while ((c1 = *s1++) == (c2 = *s2++)) {
        if (c1 == '\0')
            return 0;
    }
    return c1 - c2;
}
#include <scheduler.h>

int next_pid = 0;
int proc_counter = 0;

int started = 0;

s_node * curr;

void * scheduler(void * old_rsp){
    // printString("Entering", 8);
    // printNewLine();
    
    if(started == 0){
        printString("ERROR", 5);
        printNewLine();
        return old_rsp;
    }
    
    //curr->pcb->rsp = old_rsp;

    curr->pcb->p_counter--;
    if(curr->pcb->p_counter == 0){
        curr->pcb->p_counter = curr->pcb->priority;
        curr = curr->next;
    }

    void * new_rsp = curr->pcb->rsp;

    // printString("asd1: ", 6);
	// print64Hex( ( (uint64_t *)curr->pcb->rsp) );
	// printNewLine();

    //printPCB(curr->pcb);

    //while(1);

    return new_rsp;
}


int addPCB(void * rsp, size_t priority, void * stack_start){

    //s_pcb new_pcb = {rsp, stack_start, next_pid, priority, priority, 1};
    s_pcb * new_pcb;
    malloc(sizeof(s_pcb), &new_pcb);

    new_pcb->rsp = rsp;
    new_pcb->stack_start = stack_start;
    new_pcb->pid = next_pid;
    new_pcb->priority = priority;
    new_pcb->p_counter = priority;
    new_pcb->is_deletable = 1;
 
    next_pid++;
    
    if(started == 0){
        printString("Initializing...", 15);
        printNewLine();

        new_pcb->is_deletable = 0;

        printPCB(new_pcb);

        init(new_pcb);
        
    }else{
        printString("Adding process...", 17);
        printNewLine();
        //printPCB(&new_pcb);

        addProcess(new_pcb);
    }
    proc_counter++;
    return 0; 
}

void init(s_pcb * new_pcb){

    s_node * aux;
    malloc(sizeof(s_node), &aux);

    aux->pcb = new_pcb;
    aux->next = aux;
    aux->prev = aux;

    curr = aux;

    printString("asd2: ", 6);
	print64Hex( ( (uint64_t *)curr->pcb->rsp) );
	printNewLine();
    started = 1;

    //printPCB(new_pcb);

}

void addProcess(s_pcb * new_pcb){
    s_node * new_node;
    new_node->pcb = new_pcb;
    new_node->next = curr;
    new_node->prev = curr->prev;
    curr->prev->next = new_node;
    curr->prev = new_node;
}

int kill(uint64_t pid){
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid){
            free(aux->pcb->stack_start);
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
            proc_counter--;
            return 0;
        }
        aux = aux->next;
        counter++;
    }
    return -1;
}

void printPCB(s_pcb * pcb){
    printString("--------------PROCESS------------", 33);
    printNewLine();
    printString("rsp: ", 5);
	print64Hex( ( (uint64_t *)pcb->rsp) );
	printNewLine();
    printString("stack start: ", 13);
	print64Hex( ( (uint64_t *)pcb->stack_start ) );
	printNewLine();
    printString("pid: ", 5);
	printDec( pcb->pid );
	printNewLine();
    printString("priority: ", 10);
	printDec( pcb->priority );
	printNewLine();
    printString("priority counter: ", 18);
	printDec( pcb->p_counter );
	printNewLine();
    printString("is deletable: ", 14);
	printDec( pcb->is_deletable );
	printNewLine();
    printString("------------------------------", 30);
    printNewLine();
}

//inicia el primer proceso

#include <scheduler.h>

int next_pid = 0;
int proc_counter = 0;

int started = 0;

s_node * curr;

void * scheduler(void * old_rsp){

    if(started == 0){
        // printString("ERROR", 5);
        // printNewLine();
        return old_rsp;
    }

    if((uint64_t)old_rsp > 0x400000){
        curr->pcb->rsp = old_rsp;
    }

    curr->pcb->p_counter--;
    if(curr->pcb->p_counter == 0){
        curr->pcb->p_counter = curr->pcb->priority;

        curr = findNextReady();
    }

    void * new_rsp = curr->pcb->rsp;

    return new_rsp;
}

s_node * findNextReady(){
    int counter = 0;
    s_node * aux = curr->next;
    while (counter < proc_counter){
        if(aux->pcb->state == READY){
            return aux;
        }
        aux = aux->next;
        counter++;
    }
    return NULL;
}


int addPCB(void * rsp, size_t priority, void * stack_start, char fg, char * name){

    //s_pcb new_pcb = {rsp, stack_start, next_pid, priority, priority, 1};
    s_pcb * new_pcb;
    malloc(sizeof(s_pcb), &new_pcb);

    new_pcb->rsp = rsp;
    new_pcb->stack_start = stack_start;
    new_pcb->name = name;
    new_pcb->pid = next_pid;
    new_pcb->priority = priority;
    new_pcb->p_counter = priority;
    new_pcb->is_deletable = 1;
    new_pcb->fg = fg;
    new_pcb->state = READY;

    next_pid++;
    
    if(started == 0){
        new_pcb->caller_pid = new_pcb->pid;
        new_pcb->is_deletable = 0;

        // printString("Initializing...", 15);
        // printNewLine();
        // printPCB(new_pcb);

        init(new_pcb);
    }else{
        new_pcb->caller_pid = curr->pcb->pid;

        // printString("Adding process...", 17);
        // printNewLine();
        // printPCB(new_pcb);

        addProcess(new_pcb);

        if(fg == 1){
            blockCurrentProcess(); 
        }
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

    started = 1;
}

void addProcess(s_pcb * new_pcb){
    s_node * new_node;
    malloc(sizeof(s_node), &new_node);

    new_node->pcb = new_pcb;
    new_node->next = curr;
    new_node->prev = curr->prev;
    curr->prev->next = new_node;
    curr->prev = new_node;
}


int changePriority(uint64_t pid, char priority){
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid){
            aux->pcb->priority = priority;
            return 0;
        }
        aux = aux->next;
        counter++;
    }
    return -1;
} 


int changeState(uint64_t pid, char state){
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid){
            aux->pcb->state = state;
            return 0;
        }
        aux = aux->next;
        counter++;
    }
    return -1;
}   


int kill(uint64_t pid){
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid && aux->pcb->is_deletable == 1){
            if(aux->pcb->fg == 1){
                changeState(aux->pcb->caller_pid, READY);
            }
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

s_pcb *  getProcessInfo(uint64_t pid){
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid){
            return aux->pcb;
        }
        aux = aux->next;
        counter++;
    }
    return NULL;
} 

uint64_t getCurrentPid(){
    return curr->pcb->pid;
}

void blockCurrentProcess(){
    curr->pcb->state = BLOCKED;
}

void printProcessInfo(uint64_t pid){ 
    int counter = 0;
    s_node * aux = curr;
    while (counter < proc_counter){
        if(aux->pcb->pid == pid){
            printString("--------------------PROCESS--------------------", 47);
            printNewLine();
            printPCB(aux->pcb);
            printString("-----------------------------------------------", 47);
            printNewLine();
        }
        aux = aux->next;
        counter++;
    }
    return; 
}

void printAllProcessInfo(){
    printString("--------------------PROCESS--------------------", 47);
    printNewLine();
    int counter = 0;
        s_node * aux = curr;
        while (counter < proc_counter){
            printPCB(aux->pcb);
            aux = aux->next;
            counter++;
        }
    printString("-----------------------------------------------", 47);
    printNewLine();
    return; 
}

void printPCB(s_pcb * pcb){
    printString("name: ", 6);
	printString(pcb->name, 20);
    printString("  |  ", 5);
    printString("pid: ", 5);
	printDec( pcb->pid );
    printString("  |  ", 5);
    printString("st: ", 7);
	printDec( pcb->state );
    printString("  |  ", 5);
    printString("fg: ", 3);
	printDec( pcb->fg );
    printString("  |  ", 5);
    printString("prty: ", 10);
	printDec( pcb->priority );
    printString("  |  ", 5);
    printString("cpid: ", 12);
	printDec( pcb->caller_pid );
    printString("  |  ", 5);
    printString("sp: ", 5);
	print64Hex( ( (uint64_t *)pcb->rsp) );
	printString("  |  ", 5);
    printString("bp: ", 13);
	print64Hex( ( (uint64_t *)pcb->stack_start ) );
    printNewLine();
}


//inicia el primer proceso

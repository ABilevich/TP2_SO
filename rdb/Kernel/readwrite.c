#include <readwrite.h>

uint64_t * actual_buffer_size;
char * actual_buffer;

int sys_read_write(void * option, void * arg1, void * arg2, void * arg3, void * arg4) {
    switch ((uint64_t) option) {
	case 0:
		return rw_read((void *) arg1, (uint64_t)arg2, (uint64_t *) arg3);
		break;
	case 1:
		rw_write((void *) arg1, (uint64_t)arg2, (uint64_t *) arg3);
		break;
    }
    return 0;
}

int rw_read(void * c, uint64_t input_id, uint64_t * resp){
	//return key_read(c,input_id);
	
	pipe * aux = p_getPipe(input_id);
	if(aux == NULL){
		return 2; 
	}
	actual_buffer_size = &(aux->buff_taken_size);
	actual_buffer = aux->buff;
	
	// actual_buffer_size = p_GetBuferSize(fd);
	// actual_buffer = p_getBuffer(fd);
	//dependiendo del fd, pedirle a el driver de pipes el actual_buffer_size y actual_buffer

	if ( *actual_buffer_size <= 0){
		//blockCurrentProcessByRead();
		return 1;
	}
	char ans = actual_buffer[0]; // Devuelvo el primer char
	(*actual_buffer_size)--;
	for (int j = 0; j < *actual_buffer_size; j++) {
		actual_buffer[j] = actual_buffer[j+1]; // Muevo el buffer restante
	}
	* (char *)c = ans;
	return 0;
}

void rw_write(void * c, uint64_t input_id, uint64_t * resp){
	
}
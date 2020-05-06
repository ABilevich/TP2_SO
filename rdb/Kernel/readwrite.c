#include <readwrite.h>

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
    return key_read(c,input_id);
}

void rw_write(void * c, uint64_t input_id, uint64_t * resp){

}
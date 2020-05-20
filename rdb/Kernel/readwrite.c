#include <readwrite.h>

int sys_read_write(void *option, void *arg1, void *arg2, void *arg3, void *arg4)
{
	switch ((uint64_t)option)
	{
	case 0:
		rw_read((void *)arg1, (uint64_t)arg2, (uint64_t *)arg3);
		break;
	case 1:
		rw_write((char *)arg1, (uint64_t)arg2, (uint64_t *)arg3);
		break;
	}
	return 0;
}

void rw_read(void *c, uint64_t input_id, uint64_t *resp)
{
	pipe *aux = p_getPipe(input_id);

	if (aux == NULL)
		return;

	uint64_t *actual_buffer_size = &(aux->buff_taken_size);
	char *actual_buffer = aux->buff;

	uint64_t r_sem_id = aux->r_sem_id;
	uint64_t w_sem_id = aux->w_sem_id;

	uint64_t pid = getCurrentPid();
	uint64_t auxresp;
	addProcessToSemViaId(r_sem_id, pid);
	s_semWait(r_sem_id, pid, &auxresp);

	char ans = actual_buffer[0]; // Devuelvo el primer char
	(*actual_buffer_size)--;
	for (int j = 0; j < *actual_buffer_size; j++)
	{
		actual_buffer[j] = actual_buffer[j + 1]; // Muevo el buffer restante
	}
	*(char *)c = ans;
	s_semPost(w_sem_id, &auxresp);
	if (aux->id == 0)
	{
		s_semPost(w_sem_id, &auxresp); //por la tecla abajo y arriba si es del teclado
	}
	RemoveProcessFromSemViaId(r_sem_id, pid);
}

void rw_write(char *c, uint64_t output_id, uint64_t *resp)
{
	if (output_id == 0)
	{
		printString(c, 1);
		return;
	}

	pipe *aux = p_getPipe(output_id);

	if (aux == NULL)
		return;

	uint64_t *actual_buffer_size = &(aux->buff_taken_size);
	char *actual_buffer = aux->buff;

	uint64_t r_sem_id = aux->r_sem_id;
	uint64_t w_sem_id = aux->w_sem_id;

	uint64_t pid = getCurrentPid();
	uint64_t auxresp;

	addProcessToSemViaId(w_sem_id, pid);

	s_semWait(w_sem_id, pid, &auxresp);

	actual_buffer[(*actual_buffer_size)] = *c;
	(*actual_buffer_size)++;

	s_semPost(r_sem_id, &auxresp);

	RemoveProcessFromSemViaId(w_sem_id, pid);
}
#include <lib_user.h>

static char buffer[64] = {'0'};

static void _64Hexfill(int n, char *buffer);

// ----------- System ------------

int getMem(void *pos, uint64_t *mem_buffer, unsigned int dim)
{
	_sys_system(0, pos, (void *)mem_buffer, (void *)(uint64_t)dim);
	return 0;
}

void *malloc(int size)
{
	void **resp = NULL;
	_sys_system((void *)1, (void *)(size_t)size, resp, (void *)0);
	return *resp;
}

void free(void *pos)
{
	_sys_system((void *)2, pos, (void *)0, (void *)0);
}

int getFreeHeapSize(void)
{
	size_t *resp = 0;
	_sys_system((void *)3, (void *)resp, (void *)0, (void *)0);
	return (int)(*resp);
}

int getTotalHeapSize(void)
{
	size_t *resp = 0;
	_sys_system((void *)4, (void *)resp, (void *)0, (void *)0);
	return (int)(*resp);
}

int getTakenHeapSize(void)
{
	size_t *resp = 0;
	_sys_system((void *)5, (void *)resp, (void *)0, (void *)0);
	return (int)(*resp);
}

// ----------- Timet ------------

unsigned long getTicks()
{
	return _sys_timet(0, 0, 0);
}

unsigned long getSecondsElapsed()
{
	return _sys_timet((void *)1, 0, 0);
}

int addTimeFunction(function f, unsigned int ticks)
{
	return _sys_timet((void *)2, (void *)f, (void *)(uint64_t)ticks);
}

int updateTimeFunction(function f, unsigned int new_ticks)
{
	return _sys_timet((void *)3, (void *)f, (void *)(uint64_t)new_ticks);
}

void removeTimeFunction(function f)
{
	_sys_timet((void *)3, (void *)f, 0);
}

void wait(unsigned int millis)
{
	int last_ticks = getTicks();
	while ((getTicks() - last_ticks) < millis * 1000)
		;
}

// ----------- RTC ------------

time_struct getTime()
{
	time_struct aux;
	aux.hours = _sys_rtc((void *)2);
	aux.mins = _sys_rtc((void *)1);
	aux.secs = _sys_rtc((void *)0);
	return aux;
}

// ----------- Read ------------

int read(char *buffer, unsigned int buff_size)
{
	int finished = 0;
	int i = 0;
	uint64_t input_id = getMyInputId();
	//printf("input_id = %d\n", input_id);
	while (i < buff_size && !finished)
	{ // Mientras no se llene el buffer
		char c;
		while (_sys_read_write(0, (void *)&c, (void *)input_id, 0, 0) != 0)
			; // Mientras no consiga el caracter sigo pidiendolo.
		// printf("asd1\n");
		// _sys_read_write(0,(void *) &c,(void*)input_id,0,0);
		// printf("asd2\n");
		buffer[i++] = c;
		if (c == '\n')
			finished = 1;
	}
	return i;
}

int read_from(char *buffer, unsigned int buff_size, uint64_t input_id)
{
	int finished = 0;
	int i = 0;
	while (i < buff_size && !finished)
	{ // Mientras no se llene el buffer
		char c;
		//while (); // Mientras no consiga el caracter sigo pidiendolo.
		printf("asd1\n");
		_sys_read_write(0, (void *)&c, (void *)input_id, 0, 0);
		printf("asd2\n");
		buffer[i++] = c;
		if (c == '\n')
			finished = 1;
	}
	return i;
}

int write(char *buffer, unsigned int buff_size)
{

	return 0;
}

int write_to(char *buffer, unsigned int buff_size, uint64_t output_id)
{

	return 0;
}

int scan(char *buffer, unsigned int buff_size)
{ // Make sure u have at least buff_size + 1 to allocate the result string.
	int size_read = read(buffer, buff_size);
	buffer[size_read] = 0;
	return 0;
}

char scanChar()
{
	char c;
	read(&c, 1);
	return c;
}

// ----------- Screen ------------

void clearScreen()
{
	_sys_screen(0, 0, 0, 0);
}

void setBackgroundColor(uint32_t color)
{
	_sys_screen((void *)5, (void *)(uint64_t)color, 0, 0);
}

void setCursor(unsigned int x, unsigned int y)
{
	_sys_screen((void *)6, (void *)(uint64_t)x, (void *)(uint64_t)y, 0);
}

void shiftCursor(int offset)
{
	if (offset > 0)
		for (int i = 0; i < offset; i++)
			_sys_screen((void *)4, (void *)(uint64_t)1, 0, 0);
	else if (offset < 0)
		for (int i = 0; i < -offset; i++)
			_sys_screen((void *)4, (void *)(uint64_t)-1, 0, 0);
}

void showCursor(int status)
{
	_sys_screen((void *)3, (void *)(uint64_t)status, 0, 0);
}

int writeOnScreen(const char *str, unsigned int str_size, int color)
{
	return _sys_screen((void *)1, (void *)str, (void *)(uint64_t)str_size, (void *)(uint64_t)color);
}

int printColored(const char *str, int color)
{
	return writeOnScreen(str, strlen(str), color);
}

int putColoredChar(char c, int color)
{
	return writeOnScreen(&c, 1, color);
}

int print(const char *str)
{
	return printColored(str, WHITE_COLOR);
}

int printf(const char *format, ...)
{
	va_list pa; // Lista de parámetros
	va_start(pa, format);

	char *tmp;
	int num;

	while (*format != '\0')
	{
		if (*format != '%')
		{
			putChar(*format);
			format++;
			continue;
		}

		format++;

		switch (*format)
		{
		case 'd': // Si es un decimal
			num = va_arg(pa, int);
			if (num < 0)
			{
				putChar('-'); // Agrego signo
				num = -num;	  // Convierto en positivo
			}
			printDec(num);
			break;
		case 'x': // Si es hexadecimal
			num = va_arg(pa, unsigned int);
			printHex(num);
			break;
		case 'X':
			num = va_arg(pa, unsigned int);
			print64Hex(num);
			break;
		case 'o': // Si es octal
			num = va_arg(pa, unsigned int);
			printOct(num);
			break;
		case 'c':
			num = va_arg(pa, int);
			putChar(num);
			break;
		case 's':
			tmp = va_arg(pa, char *);
			print(tmp);
			break;
		}
		format++;
	}

	va_end(pa);
	return 0;
}

int putChar(char c)
{
	return writeOnScreen(&c, 1, WHITE_COLOR);
}

int println(const char *str)
{
	print(str);
	print("\n");
	return 0;
}

int printError(const char *str)
{
	return printColored(str, ERROR_COLOR);
}

int printDec(uint64_t value)
{
	return printBase(value, 10);
}

int printOct(uint64_t value)
{
	return printBase(value, 8);
}

int printHex(uint64_t value)
{
	return printBase(value, 16);
}

int print64Hex(uint64_t value)
{
	int digits = uintToBase(value, buffer, 16);
	_64Hexfill(16 - digits, buffer);
	return print(buffer);
}

static void _64Hexfill(int n, char *buffer)
{
	for (int i = 15; i >= 0; i--)
	{
		if (i >= n)
			buffer[i] = buffer[i - n];
		else
			buffer[i] = '0';
	}
	buffer[16] = 0;
}

int printBase(uint64_t value, uint32_t base)
{ // Had to change definition (now it returns the amount of digits, is it okay?)
	int digits = uintToBase(value, buffer, base);
	print(buffer);
	return digits;
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int deleteChar(void)
{
	return _sys_screen((void *)(uint64_t)2, 0, 0, 0);
}

int deleteNChars(int n)
{
	for (int i = 0; i < n; i++)
		deleteChar();
	return 0;
}

// ----------- Video ------------

void paintScreen(uint32_t color)
{
	_sys_video(0, (void *)(uint64_t)color, 0, 0, 0);
}

unsigned int getScreenWidth()
{
	return _sys_video((void *)(uint64_t)1, 0, 0, 0, 0);
}

unsigned int getScreenHeight()
{
	return _sys_video((void *)(uint64_t)2, 0, 0, 0, 0);
}

void drawEllipse(int x, int y, unsigned int a, unsigned int b, uint32_t color)
{
	point p = {x, y};
	_sys_video((void *)(uint64_t)3, (void *)(uint64_t)&p, (void *)(uint64_t)a, (void *)(uint64_t)b, (void *)(uint64_t)color);
}

void drawCircle(int x, int y, unsigned int r, uint32_t color)
{
	drawEllipse(x, y, r, r, color);
}

void drawRectangle(int x, int y, unsigned int b, unsigned int h, uint32_t color)
{
	point p = {x, y};
	_sys_video((void *)(uint64_t)4, (void *)(uint64_t)&p, (void *)(uint64_t)b, (void *)(uint64_t)h, (void *)(uint64_t)color);
}

void drawSquare(int x, int y, unsigned int l, uint32_t color)
{
	drawRectangle(x, y, l, l, color);
}

// ----------- Sound ------------

#define BEEP_FREQ 350

void play_timed_sound(uint32_t freq, long duration)
{
	_sys_sound((void *)(uint64_t)2, (void *)(uint64_t)freq, (void *)(uint64_t)duration);
}

void play_sound(uint32_t freq)
{
	_sys_sound((void *)(uint64_t)1, (void *)(uint64_t)freq, 0);
}

void shut_sounds()
{
	_sys_sound(0, 0, 0);
}

void beeps(uint32_t freq)
{
	play_timed_sound(freq, 1);
}

// ----------- Strings ------------

unsigned int strlen(const char *str)
{
	int i = 0;
	while (str[i++] != 0)
		;
	return i - 1;
}

int strcmp(const char *s1, const char *s2)
{
	unsigned char c1, c2;
	while ((c1 = *s1++) == (c2 = *s2++))
	{
		if (c1 == '\0')
			return 0;
	}
	return c1 - c2;
}

int strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned char c1, c2;
	int i = 1;
	while ((c1 = *s1++) == (c2 = *s2++))
	{
		if (c1 == '\0' || i == n)
			return 0;
		i++;
	}
	return c1 - c2;
}

int strcpy(char *dst, const char *src)
{
	int i = 0;
	do
	{
		dst[i] = src[i];
	} while (src[i++] != 0);
	return 0;
}

long int strtoint(char *s)
{
	long int num = 0;
	int neg = 0;
	int i = 0;

	if (s[i] == '-')
	{
		neg = 1;
		i++;
	}
	while (s[i])
	{
		num = num * 10 + s[i] - '0';
		i++;
	}

	if (neg)
		num = -num;

	return num;
}

// ----------- Process ------------

int createProcess(void *rip, uint64_t priority, char fg, char *name, uint64_t input_id, uint64_t output_id)
{
	uint64_t params[] = {priority, fg, input_id, output_id};
	_sys_process(0, rip, (void *)params, (void *)name, 0);
	return 0;
}

int changeProcessPriority(uint64_t pid, uint64_t priority)
{
	int resp = 0;
	_sys_process((void *)1, (void *)(uint64_t)pid, (void *)(uint64_t)priority, (void *)&resp, 0);
	if (resp == -1)
	{
		printf("nice ERROR: Process pid %d was not found! Try \"ps\" comand.\n", pid);
	}
	return 0;
}

int changeProcessState(uint64_t pid, char state)
{
	int resp = 0;
	_sys_process((void *)2, (void *)(uint64_t)pid, (void *)(uint64_t)state, (void *)&resp, 0);
	if (resp == -1)
	{
		printf("chstate ERROR: Process pid %d was not found! Try \"ps\" comand.\n", pid);
	}
	return 0;
}

int kill(uint64_t pid)
{
	int resp = 0;
	_sys_process((void *)3, (void *)(uint64_t)pid, (void *)&resp, 0, 0);
	if (resp == -1)
	{
		printf("kill ERROR: Process pid %d was not found! Try \"ps\" comand.\n", pid);
	}
	else if (resp == -2)
	{
		printf("kill ERROR: Process with pid %d cant be killed!\n", pid);
	}
	return 0;
}

void printProcessInfo(uint64_t pid)
{
	_sys_process((void *)4, (void *)(uint64_t)pid, 0, 0, 0);
	return;
}

void printAllProcessInfo()
{
	_sys_process((void *)5, 0, 0, 0, 0);
	return;
}

int getPid()
{
	int resp = 0;
	_sys_process((void *)6, (void *)&resp, 0, 0, 0);
	if (resp == -1)
	{
		return 0;
	}
	return resp;
}

int block(uint64_t pid)
{
	int resp = 0;
	_sys_process((void *)7, (void *)(uint64_t)pid, (void *)&resp, 0, 0);
	if (resp == -1)
	{
		printf("block ERROR: Process pid %d was not found! Try \"ps\" comand.\n", pid);
	}
	return resp;
}

uint64_t getMyInputId()
{
	uint64_t resp;
	_sys_process((void *)8, (void *)&resp, 0, 0, 0);
	return resp;
}

uint64_t getMyOutputId()
{
	uint64_t resp;
	_sys_process((void *)9, (void *)&resp, 0, 0, 0);
	return resp;
}

// ----------- Semaphore ------------

sem_info *semOpen(char *name, uint64_t start_cont)
{
	int pid = getPid();
	sem_info *aux = malloc(sizeof(sem_info));
	uint64_t resp;
	_sys_semaphore(0, (void *)name, (void *)(uint64_t)pid, (void *)(uint64_t)start_cont, (void *)&resp);
	aux->id = resp;
	return aux;
}

int semClose(sem_info *si)
{
	//printf("asd6\n");
	int pid = getPid();
	uint64_t id = si->id;
	int resp;
	_sys_semaphore((void *)1, (void *)(uint64_t)id, (void *)(uint64_t)pid, (void *)&resp, 0);
	if (resp == 1)
	{
		printf("semClose ERROR: no semaphore with id %d was found\n", id);
	}
	else if (resp == 2)
	{
		free(si);
		printf("sem destroyed\n");
	}
	return resp;
}

int semUnlink(char *name)
{
	int resp;
	_sys_semaphore((void *)2, (void *)name, (void *)(uint64_t *)&resp, 0, 0);
	if (resp == -1)
	{
		printf("semUnlink ERROR: no semaphore with name \"%s\" was found\n", name);
	}
	return resp;
}

int semWait(sem_info *si)
{
	int pid = getPid();
	uint64_t id = si->id;
	int resp;
	_sys_semaphore((void *)3, (void *)(uint64_t)id, (void *)(uint64_t)pid, (void *)(uint64_t *)&resp, 0);
	if (resp == -1)
	{
		printf("semWait ERROR: no semaphore with id %d was found\n", id);
	}
	return resp;
}

int semPost(sem_info *si)
{
	int resp;
	uint64_t id = si->id;
	_sys_semaphore((void *)4, (void *)(uint64_t)id, (void *)(uint64_t *)&resp, 0, 0);
	if (resp == 1)
	{
		printf("semPost ERROR: no semaphore with id %d was found\n", id);
	}
	if (resp == 2)
	{
		printf("semPost FATAL ERROR id: %d\n", id);
	}
	return resp;
}

void PrintAllSemInfo()
{
	_sys_semaphore((void *)5, 0, 0, 0, 0);
}

//----------------PIPES---------------------------------

pipe_info *openPipe(char *name)
{
	uint64_t resp;
	pipe_info *aux = malloc(sizeof(pipe_info));
	uint64_t pid = getPid();
	_sys_pipe(0, (void *)name, (void *)pid, (void *)&resp, 0);
	aux->id = resp;
	return aux;
}

int closePipe(pipe_info *pipe)
{
	int resp;
	uint64_t pid = getPid();
	uint64_t pipeid = pipe->id;
	_sys_pipe((void *)1, (void *)pipeid, (void *)pid, (void *)&resp, 0);
	if (resp == 1)
	{
		printf("Deleting pipe with id %d\n", pipe->id);
	}
	else if (resp == 2)
	{
		printf("closePipe ERROR: no pipe with id %d was found\n", pipe->id);
	}
	free(pipe);
	return resp;
}

void PrintAllPipeInfo()
{
	_sys_pipe((void *)2, 0, 0, 0, 0);
}

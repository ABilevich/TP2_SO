#include <lib_user.h>
#include <aracnoid.h>
#include <commands.h>
#include <shell.h>
#include <runable.h>
#include <phyloController.h>
#include <init.h>

#define BUFFER_SIZE 2000
#define COMMANDS_BUFFER_SIZE 50
#define LONGEST_PARAM 20
#define MAX_PARAMS 3
#define STUCKED_BEEP_FREQ 230

#define FG 1
#define BG 0
#define STDIN 0
#define STDOUT 0

#define USER_COLOR 0xfff4a3
#define USER_BACKGROUND_COLOR 0x000000

#define TAB '\t'

typedef void (*fn)();

static int command_launch(char *cmd);
static int getRunableIndex(char *cmd);
static int instructionReader(char *cmd, char params[][LONGEST_PARAM]);
static void specialKeyHandler(void);
static void userWritingHandler(void);
static void userDeletingBackwardsHandler(void);
static void userDeletingTowardsHandler(void);
static int findCommand(void);
static void instructionHandler(void);
static void welcomeMessage(void);

static char inputBuffer[BUFFER_SIZE];
static char commandsHistory[COMMANDS_BUFFER_SIZE][BUFFER_SIZE];
static char *commands[] = {"aracnoid", "clear", "clock", "help", "inforeg", "printmem", "set", "set writing_color", "test", "test zero_div", "test inv_op_code", "test mem", "test proc", "nice", "run", "ps", "mem", "sem", "pipe"};
static char *void_func[] = {"help", "clock", "inforeg", "clear", "ps", "mem", "sem", "pipe", "phylo", "a", "r"};
static void (*void_commands_func[])(void) = {printUserManual, getLocalTime, printRegistersInfo, clear, ps, mem, sem, pipe, startPhyloController, addPhylo, removePhylo};

static char *runable_name[] = {"a", "b", "c", "d", "e", "test_mem", "test_sem", "loop", "test_proc", "test_prio", "test_sem2", "wc", "filter", "cat", "test_sync", "test_async"};
static void (*runable_func[])(void) = {start_a, start_b, start_c, start_d, start_e, test_mm, test_sync_async, start_loop, test_processes, test_prio, test_sem, start_wc, start_filter, start_cat, run_test_sync, run_test_async};

static char *user = "dummie_user";
static char *syst_name = "@rdb: ";
static int user_writing_color;

static int abnormalInsertion;

static int commandsHistory_size;
static int commandsHistory_index;

static int user_cursor_pos;
static int user_string_size;

static int c;

static gameState aracnoid_save;
static int aracnoid_saved;
void startShell()
{
    void (*init)(void);
    init = &startInit;
    createProcess(init, 1, 0, "init", STDIN, STDOUT);
    changeProcessState(INIT_PID, 5);

    setCursor(0, 0);
    welcomeMessage();

    setCursor(0, getScreenHeight());
    user_writing_color = USER_COLOR;
    setBackgroundColor(USER_BACKGROUND_COLOR);
    int real_buff_size = BUFFER_SIZE - strlen(user) - strlen(syst_name);

    while (1)
    {
        showCursor(1);
        printColored(user, 0xFFC550);
        printColored(syst_name, 0xF0A100);
        while (((c = scanChar()) != '\n') && (user_string_size < real_buff_size))
        {
            specialKeyHandler();
            if (c != TAB && c > 0)
            { // If it´s not a special key
                userWritingHandler();
            }
        }
        println(""); // Enter antes de ejecutar lo que sea
        if (user_cursor_pos == real_buff_size)
        {
            // PIP
            char *err_message = "You've exceeded buffer size, try again.";
            printError(err_message);
            println("");
        }
        else
        {
            showCursor(0);
            inputBuffer[user_string_size] = 0;
            if (inputBuffer[0] != 0 && (commandsHistory_size == 0 || (strcmp(inputBuffer, commandsHistory[commandsHistory_size - 1]) != 0)))
            { // Si no se escribe nada o es igual al ultimo comando no lo guardo
                strcpy(commandsHistory[commandsHistory_size++], inputBuffer);
            }
            instructionHandler();
        }
        user_string_size = user_cursor_pos = 0;
        commandsHistory_index = commandsHistory_size - 1;
    }
}

static void userWritingHandler()
{
    if (abnormalInsertion)
    {
        if (user_cursor_pos == user_string_size)
            user_string_size++;
        inputBuffer[user_cursor_pos++] = c;
        putColoredChar((char)c, user_writing_color);
    }
    else
    {
        for (int i = user_string_size; i > user_cursor_pos; i--)
            inputBuffer[i] = inputBuffer[i - 1];
        inputBuffer[user_cursor_pos++] = c;
        user_string_size++;
        putColoredChar(c, user_writing_color);
        for (int j = user_cursor_pos; j < user_string_size; j++)
        {
            putColoredChar(inputBuffer[j], user_writing_color);
        }
        shiftCursor(user_cursor_pos - user_string_size);
    }
}

static void specialKeyHandler()
{
    int aux;

    switch (c)
    {

    case INS:
        abnormalInsertion = 1 - abnormalInsertion;
        break;

    case BACKS:
        if (user_cursor_pos > 0)
        {
            userDeletingBackwardsHandler();
        }
        break;

    case DEL:
        if (user_cursor_pos < user_string_size)
        {
            userDeletingTowardsHandler();
        }
        break;

    case ARROW_DOWN:
        if (commandsHistory_index < commandsHistory_size - 1)
        {
            if (user_string_size > user_cursor_pos)
                shiftCursor(user_string_size - user_cursor_pos);
            deleteNChars(strlen(commandsHistory[++commandsHistory_index]));
            strcpy(inputBuffer, commandsHistory[commandsHistory_index + 1]);
            user_string_size = user_cursor_pos = strlen(commandsHistory[commandsHistory_index + 1]);
            printColored(commandsHistory[commandsHistory_index + 1], user_writing_color);
        }
        break;

    case ARROW_UP:
        if (commandsHistory_index >= 0)
        {
            if (user_string_size > user_cursor_pos)
                shiftCursor(user_string_size - user_cursor_pos);
            if (commandsHistory_index == commandsHistory_size - 1)
            {
                deleteNChars(user_string_size);
            }
            else
            {
                deleteNChars(strlen(commandsHistory[commandsHistory_index + 1]));
            }
            strcpy(inputBuffer, commandsHistory[commandsHistory_index]);
            user_string_size = user_cursor_pos = strlen(commandsHistory[commandsHistory_index]);
            printColored(commandsHistory[commandsHistory_index--], user_writing_color);
        }
        break;

    case ARROW_LEFT:
        if (user_cursor_pos > 0)
        {
            user_cursor_pos--;
            shiftCursor(-1);
        }
        break;

    case ARROW_RIGHT:
        if (user_cursor_pos < user_string_size)
        {
            user_cursor_pos++;
            shiftCursor(1);
        }
        break;

    case TAB:
        aux = findCommand();
        if (aux >= 0)
        {
            if (user_string_size > user_cursor_pos)
                shiftCursor(user_string_size - user_cursor_pos);
            deleteNChars(user_string_size);
            strcpy(inputBuffer, commands[aux]);
            user_string_size = user_cursor_pos = strlen(commands[aux]);
            printColored(commands[aux], user_writing_color);
        }
        break;
    }
}

static void userDeletingBackwardsHandler()
{
    user_cursor_pos--;
    user_string_size--;
    if (user_cursor_pos == user_string_size)
    {
        deleteChar();
    }
    else
    {
        shiftCursor(-1);
        for (int i = user_cursor_pos; i < user_string_size; i++)
        {
            inputBuffer[i] = inputBuffer[i + 1];
            putColoredChar(inputBuffer[i + 1], user_writing_color);
        }
        putChar(' ');
        shiftCursor(user_cursor_pos - user_string_size - 1);
    }
}

static void userDeletingTowardsHandler()
{
    for (int i = user_cursor_pos; i < user_string_size - 1; i++)
    {
        inputBuffer[i] = inputBuffer[i + 1];
        putColoredChar(inputBuffer[i + 1], user_writing_color);
    }
    putChar(' ');
    shiftCursor(user_cursor_pos - user_string_size);
    user_string_size--;
}

static int findCommand()
{
    for (int i = 0; i < sizeof(commands) / sizeof(char *); i++)
        if (strncmp(commands[i], inputBuffer, user_string_size) == 0)
            return i;
    return -1;
}

static void instructionHandler()
{
    char cmd[COMMANDS_BUFFER_SIZE];
    char params[MAX_PARAMS][LONGEST_PARAM];

    int res = instructionReader(cmd, params); // returns params_read if params_read <= MAX_PARAMS, -1 if params_read > MAX_PARAMS

    int executed = 0;

    switch (res)
    {

    case 0:
        if (strcmp(cmd, "aracnoid") == 0)
        {
            startAracnoid(&aracnoid_save, &aracnoid_saved);
            if (aracnoid_saved)
                printColored("\n                                    Aracnoid is saved! type \"aracnoid\" to resume the game.\n\n", 0x04E798);
            executed = 0;
        }
        else
            executed = command_launch(cmd);
        break;
    case 1:
        if (strcmp(cmd, "printmem") == 0)
        {
            uint64_t aux = strtoint(params[0]);
            printMemoryStatus(aux);
        }
        else if (strcmp(cmd, "test") == 0)
            test(params[0]);
        else if (strcmp(cmd, "kill") == 0)
        {
            uint64_t aux = strtoint(params[0]);
            p_kill(aux);
        }
        else if (strcmp(cmd, "block") == 0)
        {
            uint64_t aux = strtoint(params[0]);
            p_block(aux);
        }
        else if (strcmp(cmd, "run") == 0)
        {
            int i = getRunableIndex(params[0]);
            if (i == -1)
            {
                printf("run ERROR: no runable with that name found!\n");
            }
            else
            {
                fn func = runable_func[i];
                char *name = runable_name[i];
                run(func, name, FG, STDIN, STDOUT);
            }
        }
        else
            executed = 1;
        break;
    case 2:
        if (strcmp(cmd, "set") == 0)
            command_set(params[0], params[1]);
        else if (strcmp(cmd, "chstate") == 0)
        {
            uint64_t aux1 = strtoint(params[0]);
            uint64_t aux2 = strtoint(params[1]);
            chstate(aux1, aux2);
        }
        else if (strcmp(cmd, "nice") == 0)
        {
            uint64_t aux1 = strtoint(params[0]);
            uint64_t aux2 = strtoint(params[1]);
            nice(aux1, aux2);
        }
        else if (strcmp(cmd, "run") == 0)
        {
            int i = getRunableIndex(params[0]);
            if (i == -1)
            {
                printf("run ERROR: no runable with that name found!\n");
            }
            else
            {
                fn func = runable_func[i];
                char *name = runable_name[i];
                char fg = strcmp(params[1], "&") == 0 ? BG : FG;

                run(func, name, fg, STDIN, STDOUT);
            }
        }
        else
            executed = 1;
        break;
    case 3:
        if (strcmp(cmd, "run") == 0)
        {
            if (strcmp(params[1], "|") != 0)
            {
                printf("run ERROR: unknown symbol!\n");
            }
            else
            {
                int i1 = getRunableIndex(params[0]);
                int i2 = getRunableIndex(params[2]);
                if (i1 == -1)
                {
                    printf("run ERROR: no runable with name: %s found!\n", params[0]);
                }
                else if (i2 == -1)
                {
                    printf("run ERROR: no runable with name: %s found!\n", params[2]);
                }
                else
                {
                    fn func1 = runable_func[i1];
                    char *name1 = runable_name[i1];
                    fn func2 = runable_func[i2];
                    char *name2 = runable_name[i2];

                    pipe_info *pipe = openPipe(NULL);
                    run(func1, name1, BG, STDIN, pipe->id);
                    run(func2, name2, BG, pipe->id, STDOUT);
                    closePipe(pipe);
                }
            }
        }
        else
        {
            executed = 1;
        }
        break;
    }

    if ((res == -1 || executed != 0) && inputBuffer[0] != 0)
    {
        printError("Command not found.\n");
    }
}

static int instructionReader(char *cmd, char params[][LONGEST_PARAM])
{
    int params_read = 0, j = 0, i = 0;
    char *instr = inputBuffer;

    //building command
    while (instr[i] != '\0' && instr[i] != ' ' && i < COMMANDS_BUFFER_SIZE)
    {
        cmd[i] = instr[i];
        i++;
    }
    cmd[i] = '\0';

    if (instr[i] == '\0')
        return params_read;

    if (i < COMMANDS_BUFFER_SIZE)
    {
        i++;
    }
    //building params
    while (instr[i] != '\0' && params_read <= MAX_PARAMS)
    {
        if (instr[i] != ' ')
        {
            params[params_read][j++] = instr[i];
        }
        else
        {
            params[params_read++][j] = '\0';
            j = 0;
        }
        i++;
    }
    if (instr[i] == '\0')
        params[params_read++][j] = '\0';

    if (params_read > MAX_PARAMS)
        return -1;
    return params_read;
}

static int command_launch(char *cmd)
{

    int i, dim = sizeof(void_commands_func) / sizeof(void_commands_func[0]);

    for (i = 0; i < dim; i++)
    {
        if (strcmp(cmd, void_func[i]) == 0)
        {
            void_commands_func[i]();
            return 0;
        }
    }

    return 1;
}

static int getRunableIndex(char *cmd)
{

    int i, dim = sizeof(runable_func) / sizeof(runable_func[0]);

    for (i = 0; i < dim; i++)
    {
        if (strcmp(cmd, runable_name[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

void setUserWritingColor(uint32_t color)
{
    user_writing_color = color;
}

void resetUserWritingColor()
{
    user_writing_color = USER_COLOR;
}

static void welcomeMessage()
{

    println("");
    println("");
    println("");
    println("");
    println("               (    (        )     *                             (         )   (    ");
    println("               )\\ ) )\\ )  ( /(   (  `          *   )             )\\ )   ( /(   )\\ )  ");
    println("              (()/((()/(  )\\())  )\\))(   (   ` )  /( (       (  (()/(   )\\()) (()/(  ");
    println("               /(_))/(_))((_)\\  ((_)()\\  )\\   ( )(_)))\\      )\\  /(_)) ((_)\\   /(_)) ");
    println("              (_)) (_))    ((_) (_()((_)((_) (_(_())((_)  _ ((_)(_))     ((_) (_))   ");
    println("              | _ \\| _ \\  / _ \\ |  \\/  || __||_   _|| __|| | | |/ __|   / _ \\ / __|  ");
    println("              |  _/|   / | (_) || |\\/| || _|   | |  | _| | |_| |\\__ \\  | (_) |\\__ \\  ");
    println("              |_|  |_|_\\  \\___/ |_|  |_||___|  |_|  |___| \\___/ |___/   \\___/ |___/  ");

    println("                                           _");
    println("                                        ==(W{==========-      /===-");
    println("                                          ||  (.--.)         /===-_---~~~~~~~----__");
    println("                                          | \\_,|**|,__      |===-~___            _,-'`");
    println("                             -==\\\\        `\\ ' `--'   ),    `//~\\\\   ~~~~`--._.-~");
    println("                         ______-==|        /`\\_. .__/\\ \\    | |  \\\\          _-~`");
    println("                   __--~~~  ,-/-==\\      (   | .  |~~~~|   | |   `\\       ,'");
    println("                _-~       /'    |  \\     )__/==0==-\\<>/   / /      \\     /");
    println("              .'        /       |   \\      /~\\___/~~\\/  /' /        \\   /");
    println("             /  ____  /         |    \\`\\.__/-~~   \\  |_/'  /          \\/'");
    println("            /-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`");
    println("                              \\_|      /        _) | ;  ),   __--~~");
    println("                                '~~--_/      _-~/- |/ \\   '-~ \\");
    println("                               {\\__--_/}    / \\\\_>-|)<__\\      \\");
    println("                               /'   (_/  _-~  | |__>--<__|      |");
    println("                              |   _/) )-~     | |__>--<__|      |");
    println("                              / /~ ,_/       / /__>---<__/      |");
    println("                             o-o _//        /-~_>---<__-~      /");
    println("                             (^(~          /~_>---<__-      _-~");
    println("                            ,/|           /__>--<__/     _-~");
    println("                         ,//('(          |__>--<__|     /                  .--_");
    println("                        ( ( '))          |__>--<__|    |                 /' _-_~\\");
    println("                     `-)) )) (           |__>--<__|    |                /'  /   ~\\`\\");
    println("                    ,/,'//( (             \\__>--<__\\    \\            /'  //      ||");
    println("                  ,( ( ((, ))              ~-__>--<_~-_  ~--__---~'/'/  /'       VV");
    println("                `~/  )` ) ,/|                 ~-_~>--<_/-__      __-~ _/");
    println("              ._-~//( )/ )) `                    ~~-'_/_/ /~~~~~__--~");
    println("               ;'( ')/ ,)(                              ~~~~~~~~");
    println("              ' ') '( (/");

    println("");
    println("");
}

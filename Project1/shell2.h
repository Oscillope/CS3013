//Jason Rosenman

#define CMD_OK 0
#define CMD_BUILTIN 1
#define CMD_NEWLN 2
#define CMD_LONG 3
#define CMD_BG 4
#define CMD_EXIT 5
#define CMD_JOBS 6
#define CMD_ERR -1
#define TRUE 0
#define FALSE 1

int getCommand(char** args);
int makeChild(char** args);
int checkCmd(char** args, int nargs);

typedef struct {
	int pid;
	int jobn;
	struct timeval before;
	struct timeval after;
	char* name;
	char running;
} job;

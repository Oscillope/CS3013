// Jason Rosenman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

void getCommand(char** args);
int makeChild(char** args);
void checkCmd(char** args);

int main(int argc, char** argv) {
	int status, pid;
	struct timeval before;
	struct rusage usage;
	struct timeval after;
	long int cputime, usertime, realtime;
	while(1) {
		char** args = malloc(sizeof(char*)*500);
		getCommand(args);
		checkCmd(args);
		gettimeofday(&before, NULL);
		pid = makeChild(args);
		wait(&status);
		gettimeofday(&after, NULL);
		getrusage(RUSAGE_CHILDREN, &usage);
		free(args);
		#ifdef DEBUG
			printf("\nChild %d died with status %d.\n\n", pid, status);
		#endif
		cputime = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);
		usertime = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
		realtime = ((after.tv_sec * 1000) + (after.tv_usec / 1000)) - ((before.tv_sec * 1000) + (before.tv_usec / 1000));
		printf("Process statistics for #%d: \n", pid);
		printf("	Elapsed Time: %ld ms\n", realtime);
		printf("	CPU Time: %ld ms\n", cputime);
		printf("	User Time: %ld ms\n", usertime);
		printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw);
		printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw);
		printf("	Page Faults: %ld\n", usage.ru_majflt);
		printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt);
		if(status != 0)
			printf("\nProcess exited with status: %d\n", status);
	}
	return 0;
}

void getCommand(char** args) {
	printf("==] ");
	char* input = malloc(sizeof(char)*500);
	fgets(input, 500, stdin);
	int i = 0;
	char* temp = malloc(sizeof(char)*500);
	args[0] = strdup(strtok(input, " "));
	#ifdef DEBUG
		printf("Arg 0: %s\n", args[0]);
	#endif
	while((temp = strtok(NULL, " ")) != NULL) {
		i++;
		#ifdef DEBUG
			printf("Arg %d: %s\n", i, temp);
		#endif
		args[i] = strdup(temp);
	}
	args[i][strlen(args[i])-1] = '\0';
	args[i+1] = NULL;
	free(input);
	free(temp);
	return;
}

int makeChild(char** args) {
	int cpid = fork();
	if(cpid == 0) {
		#ifdef DEBUG
			printf("Command: %s\n", args[0]);
		#endif
		int error = execvp(args[0], args);
		if(error == -1) {
			if(errno == ENOENT)
				fprintf(stderr, "Command not found.\n");
			else
				fprintf(stderr, "An error occurred while executing the command. Please try again.\n");	
		}
	}
	return cpid;
}

void checkCmd(char** args) {
	if(strcmp(args[0], "exit") == 0)
		exit(0);
	else if(strcmp(args[0], "cd") == 0) {
		if(chdir(args[1]) == -1)
			switch(errno) {
				case EACCES:
					fprintf(stderr, "Permission denied.\n");
					break;
				case ELOOP:
					fprintf(stderr, "Symbolic link loop detected.\n");
					break;
				case ENAMETOOLONG:
					fprintf(stderr, "Path name too long.\n");
					break;
				case ENOENT:
					fprintf(stderr, "Path not found.\n");
					break;
				case ENOTDIR:
					fprintf(stderr, "Not a directory.\n");
					break;
			};
		free(args);
		args = malloc(sizeof(char*)*500);
		getCommand(args);
	}
	return;
}

// Jason Rosenman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//Jason Rosenman

#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "malloc.h"

#define CMD_OK 0
#define CMD_CD 1
#define CMD_NEWLN 2
#define CMD_LONG 3
#define CMD_ERR -1

int getCommand(char** args);
int makeChild(char** args);
int checkCmd(char** args);

int main(int argc, char** argv) {
	int status, pid;
	struct timeval before;
	struct rusage usage, old_usage;
	struct timeval after;
	long int cputime, usertime, realtime, old_cputime = 0, old_usertime = 0;
	getrusage(RUSAGE_CHILDREN, &old_usage);
	while(1) {
		char** args = malloc(sizeof(char*)*65);
		if(getCommand(args) == CMD_OK) {
			gettimeofday(&before, NULL);
			pid = makeChild(args);
			wait(&status);
			if(pid == CMD_ERR)
				exit(1);
			gettimeofday(&after, NULL);
			getrusage(RUSAGE_CHILDREN, &usage);
			free(args);
			#ifdef DEBUG
				printf("\nChild %d died with status %d.\n\n", pid, status);
			#endif
			if(WEXITSTATUS(status) != 1) {
				cputime = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);
				usertime = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
				realtime = ((after.tv_sec * 1000) + (after.tv_usec / 1000)) - ((before.tv_sec * 1000) + (before.tv_usec / 1000));
				printf("\nProcess statistics for #%d: \n", pid);
				printf("	Elapsed Time: %ld ms\n", realtime);
				printf("	CPU Time: %ld ms\n", cputime - old_cputime);
				printf("	User Time: %ld ms\n", usertime - old_usertime);
				printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw - old_usage.ru_nivcsw);
				printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw - old_usage.ru_nvcsw);
				printf("	Page Faults: %ld\n", usage.ru_majflt - old_usage.ru_majflt);
				printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt - old_usage.ru_minflt);
				if(WEXITSTATUS(status) != 0)
					printf("\nProcess exited with status: %d\n", WEXITSTATUS(status));
			}
			old_usage = usage;
			old_cputime = cputime;
			old_usertime = usertime;
			printf("\n");
		}
	}
	return 0;
}

int getCommand(char** args) {
	printf("==] ");
	char* input = malloc(sizeof(char)*130);
	fgets(input, 130, stdin);
	if(feof(stdin)) {
		free(args);
		exit(0);
	}
	#ifdef DEBUG
		printf("Length: %d Last character: %c\n", strlen(input), input[128]);
	#endif
	if(strlen(input) == 129 && input[128] != '\n') {
		fprintf(stderr, "Input string too long.\n");
		while(input[strlen(input)-1] != '\n')
			fgets(input, 130, stdin);
		free(input);
		return CMD_LONG;
	}
			
	int i = 0;
	char* temp;
	if((temp = strtok(input, " \n")) == NULL) {
		#ifdef DEBUG
			printf("Stray newline detected!\n");
		#endif
		printf("\n");
		args[0] = NULL;
		return CMD_NEWLN;
	}
	else args[0] = strdup(temp);
	#ifdef DEBUG
		printf("Arg 0: %s\n", args[0]);
	#endif
	while((temp = strtok(NULL, " \n")) != NULL) {
		i++;
		#ifdef DEBUG
			printf("Arg %d: %s\n", i, temp);
		#endif
		args[i] = strdup(temp);
	}
	args[i+1] = NULL;
	free(input);
	return checkCmd(args);
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
			return CMD_ERR;
		}
	}
	return cpid;
}

int checkCmd(char** args) {
	if(strcmp(args[0], "exit") == 0) {
		free(args);
		exit(0);
	}
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
		args[0] = NULL;
		return CMD_CD;
	}
	return CMD_OK;
}

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
#include "shell2.h"

int main(int argc, char** argv) {
	int status, pid, cmd, jobs = 0, running = 0;
	job** scheduler = malloc(sizeof(job*));
	struct timeval before;
	struct rusage usage;
	struct timeval after;
	long int cputime, usertime, realtime;
	while(1) {
		char** args = malloc(sizeof(char*)*65);
		cmd = getCommand(args);
		if(cmd == CMD_OK) {
			gettimeofday(&before, NULL);
			pid = makeChild(args);
			wait4(pid, &status, 0, &usage);
			if(pid == CMD_ERR)
				exit(1);
			gettimeofday(&after, NULL);
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
				printf("	CPU Time: %ld ms\n", cputime);
				printf("	User Time: %ld ms\n", usertime);
				printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw);
				printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw);
				printf("	Page Faults: %ld\n", usage.ru_majflt);
				printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt);
				if(WEXITSTATUS(status) != 0)
					printf("\nProcess exited with status: %d\n", WEXITSTATUS(status));
			}
			printf("\n");
		}
		else if(cmd == CMD_BG) {
			scheduler[jobs] = malloc(sizeof(job));
			scheduler[jobs]->name = strdup(args[0]);
			scheduler[jobs]->pid = makeChild(args);
			if(scheduler[jobs]->pid == CMD_ERR) {
				free(scheduler[jobs]->name);
				free(scheduler[jobs]);
				exit(1);
			}
			else {
				scheduler[jobs]->running = TRUE;
				free(args);
				scheduler[jobs]->jobn = jobs + 1;
				printf("[%d] %d\n", scheduler[jobs]->jobn, scheduler[jobs]->pid);
				gettimeofday(&(scheduler[jobs]->before), NULL);
				jobs++;
				running++;
				scheduler = realloc(scheduler, sizeof(job*)*(jobs+1));
			}
		}
		else if(cmd == CMD_JOBS) {
			int i;
			for(i = 0; i < jobs; i++) {
				if(scheduler[i]->running == TRUE)
					printf("[%d] %d %s\n", scheduler[i]->jobn, scheduler[i]->pid, scheduler[i]->name);
			}
		}
		else if(cmd == CMD_EXIT || cmd == CMD_EOF) {
			if(running != 0) {
				fprintf(stderr, "There are still jobs waiting to complete.\n");
				if(cmd == CMD_EOF) {
					job* curjob;
					int i;
					for(i = 0; i < jobs; i++) {
						if(scheduler[i]->running == TRUE)
							curjob = scheduler[i];
							printf("Waiting on job %d (%s).\n", curjob->jobn, curjob->name);
							wait4(curjob->pid, &status, 0, &usage);
							gettimeofday(&(curjob->after), NULL);
							#ifdef DEBUG
								printf("\nChild %d died with status %d.\n\n", pid, status);
							#endif
							if(WEXITSTATUS(status) != 1) {
								cputime = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);
								usertime = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
								realtime = ((curjob->after.tv_sec * 1000) + (curjob->after.tv_usec / 1000)) - ((curjob->before.tv_sec * 1000) + (curjob->before.tv_usec / 1000));
								printf("\n--> Background job %d (%s) complete.\n", curjob->jobn, curjob->name);
								printf("Process statistics for #%d: \n", pid);
								printf("	Elapsed Time: %ld ms\n", realtime);
								printf("	CPU Time: %ld ms\n", cputime);
								printf("	User Time: %ld ms\n", usertime);
								printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw );
								printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw);
								printf("	Page Faults: %ld\n", usage.ru_majflt);
								printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt);
								if(WEXITSTATUS(status) != 0)
									printf("\nProcess exited with status: %d\n", WEXITSTATUS(status));
							}
							printf("\n");
							curjob->running = FALSE;
							running--;
					}
				}
			}
			else {
				int i;
				for(i = 0; i < jobs; i++) {
					free(scheduler[i]->name);
					free(scheduler[i]);
				}
				free(args);
				free(scheduler);
				exit(0);
			}
		}
		while((pid = wait3(&status, WNOHANG, &usage)) != -1 && pid != 0) {
			job* curjob;
			int i;
			for(i = 0; i < jobs; i++) {
				if(scheduler[i]->pid == pid) {
					curjob = scheduler[i];
					break;
				}
			}
			gettimeofday(&(curjob->after), NULL);
			#ifdef DEBUG
				printf("\nChild %d died with status %d.\n\n", pid, status);
			#endif
			if(WEXITSTATUS(status) != 1) {
				cputime = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);
				usertime = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
				realtime = ((curjob->after.tv_sec * 1000) + (curjob->after.tv_usec / 1000)) - ((curjob->before.tv_sec * 1000) + (curjob->before.tv_usec / 1000));
				printf("\n--> Background job %d (%s) complete.\n", curjob->jobn, curjob->name);
				printf("Process statistics for #%d: \n", pid);
				printf("	Elapsed Time: %ld ms\n", realtime);
				printf("	CPU Time: %ld ms\n", cputime);
				printf("	User Time: %ld ms\n", usertime);
				printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw );
				printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw);
				printf("	Page Faults: %ld\n", usage.ru_majflt);
				printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt);
				if(WEXITSTATUS(status) != 0)
					printf("\nProcess exited with status: %d\n", WEXITSTATUS(status));
			}
			printf("\n");
			curjob->running = FALSE;
			running--;
		}
	}
	return 0;
}

int getCommand(char** args) {
	printf("==] ");
	char* input = malloc(sizeof(char)*130);
	fgets(input, 130, stdin);
	if(feof(stdin)) {
		return CMD_EOF;
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
	return checkCmd(args, i);
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

int checkCmd(char** args, int nargs) {
	if(strcmp(args[0], "exit") == 0) {
		return CMD_EXIT;
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
		return CMD_BUILTIN;
	}
	else if(strcmp(args[0], "jobs") == 0)
		return CMD_JOBS;
	else if(args[nargs][strlen(args[nargs])-1] == '&') {
		args[nargs] = NULL;
		return CMD_BG;
	}
	return CMD_OK;
}

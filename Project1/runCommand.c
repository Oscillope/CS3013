// Kristen Brann & Jason Rosenman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
	int status, pid;
	pid = fork();
	if(pid == 0) {
		int error = execvp(argv[0], ++argv);
		if(error == -1) {
			if(errno == ENOENT) {
					printf("Filename or path not found.\n");
					return errno;
			}
			else return errno;
		}
	}
	wait(&status);
	printf("Child %d died with status %d.\n", pid, status);
	return status;
}

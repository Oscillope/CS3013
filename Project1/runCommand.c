// Kristen Brann & Jason Rosenman

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
	int status, pid;
	argv++;
	struct timeval before;
	gettimeofday(&before, NULL);
	pid = fork();
	if(pid == 0) {
		int error = execvp(argv[0], argv);
		if(error == -1) {
			if(errno == ENOENT) {
					printf("Filename or path not found.\n");
					return errno;
			}
			else return errno;
		}
	}
	wait(&status);
	struct rusage usage;
	struct timeval after;
	gettimeofday(&after, NULL);
	getrusage(RUSAGE_CHILDREN, &usage);
	printf("\nChild %d died with status %d.\n\n", pid, status);
	long int cputime = (usage.ru_stime.tv_sec * 1000) + (usage.ru_stime.tv_usec / 1000);
	long int usertime = (usage.ru_utime.tv_sec * 1000) + (usage.ru_utime.tv_usec / 1000);
	long int realtime = ((after.tv_sec * 1000) + (after.tv_usec / 1000)) - ((before.tv_sec * 1000) + (before.tv_usec / 1000));
	printf("Process statistics: \n");
	printf("	Elapsed Time: %ld ms\n", realtime);
	printf("	CPU Time: %ld ms\n", cputime);
	printf("	User Time: %ld ms\n", usertime);
	printf("	Involuntary Context Switches: %ld\n", usage.ru_nivcsw);
	printf("	Voluntary Context Switches: %ld\n", usage.ru_nvcsw);
	printf("	Page Faults: %ld\n", usage.ru_majflt);
	printf("	Reclaimed Page Faults: %ld\n", usage.ru_minflt);
	return status;
}

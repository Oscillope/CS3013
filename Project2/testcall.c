#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "processinfo.h"
// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 349
#define __NR_cs3013_syscall2 350
#define __NR_cs3013_syscall3 351

long testCall1 ( void) {
	return (long) syscall(__NR_cs3013_syscall1);
}

long testCall2 (struct processinfo* info) {
	return (long) syscall(__NR_cs3013_syscall2, info);
}

int main () {
	struct processinfo* info = malloc(sizeof(struct processinfo));
	printf("The return values of the system calls are:\n");
	printf("\tcs3013_syscall1: %ld\n", testCall1());
	printf("\tcs3013_syscall2: %ld\n", testCall2(info));
	printf("_________________________________\n");
	printf("|\tProcess statistics\t|\n");
	printf("|PID: %d\t\t\t|\n", info->pid);
	printf("|Parent PID: %d\t\t|\n", info->parent_pid);
	printf("|Youngest child PID: %d\t\t|\n", info->youngest_child);
	printf("|Oldest sibling PID: %d\t|\n", info->older_sibling);
	printf("|Start time: %lld\t|\n", info->start_time);
	printf("|User time: %lld\t\t|\n", info->user_time);
	printf("|System time: %lld\t\t\t|\n", info->sys_time);
	printf("|Child user time: %lld\t\t|\n", info->cutime);
	printf("|Child system time: %lld\t\t|\n", info->cstime);
	printf("_________________________________\n");
	free(info);
	return 0;
}

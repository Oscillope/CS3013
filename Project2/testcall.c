#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 349
#define __NR_cs3013_syscall2 350
#define __NR_cs3013_syscall3 351

struct processinfo {
    long state;             // current state of process
    pid_t pid;              // process ID of this process
    pid_t parent_pid;       // process ID of parent
    pid_t youngest_child;   // process ID of youngest child
    pid_t younger_sibling;  // pid of next younger sibling
    pid_t older_sibling;    // pid of next older sibling
    uid_t uid;              // user ID of process owner
    long long start_time;   // process start time in nanoseconds since boot time
    long long user_time;    // CPU time in user mode (microseconds)
    long long sys_time;     // CPU time in system mode (microseconds)
    long long cutime;       // user time of children (microseconds)
    long long cstime;       // system time of children (microseconds)
};

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
	printf("pid: %d\n", info->pid);
	printf("parent pid: %d\n", info->parent_pid);
	printf("youngest child pid: %d\n", info->youngest_child);
	return 0;
}

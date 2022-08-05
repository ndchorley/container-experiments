#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int runShell(void *arg) {
    char *arguments[] = {"bash", NULL};
    char *environment[] = {NULL};
    execve("/bin/bash", arguments, environment);

    printf("After execve - something went wrong\n");

    return 0;
}

int main() {
    printf("Running a shell...\n\n");

    int stackSize = 65536;
    char *stackStart = malloc(stackSize);
    char *stackEnd = stackStart + stackSize;

    int flags = CLONE_FILES | SIGCHLD;
    pid_t childPid =
        clone(
            runShell,
            stackStart,
            flags,
            NULL
        );

    waitpid(childPid, NULL, 0);

    printf("\nShell terminated\n");

    return 0;
}

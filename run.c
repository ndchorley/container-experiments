#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    if (childPid == -1) {
        printf("clone failed: ", strerror(errno));
    }

    waitpid(childPid, NULL, 0);

    printf("\nShell terminated\n");

    return 0;
}

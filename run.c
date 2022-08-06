#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <unistd.h>

void changeRootDirectory() {
    int result = chroot("./container_root");

    if (result == -1) {
        printf(
            "Changing root directory failed: %s",
            strerror(errno)
        );

        exit(result);
    }
}

void changeWorkingDirectoryToRoot() {
    int result = chdir("/");

    if (result == -1) {
        printf(
            "Changing working directory to root failed: %s",
            strerror(errno)
        );

        exit(result);
    }
}

void startShell() {
    char *arguments[] = {"sh", NULL};
    char *environment[] = {NULL};
    int result = execve("/bin/sh", arguments, environment);

    if (result == -1) {
        printf("Running shell failed: %s", strerror(errno));
        exit(result);
    }
}

void mountProcFilesystem() {
    int result = mount("proc", "/proc", "proc", 0, NULL);

    if (result == -1) {
        printf("Mounting proc filesystem failed: %s", strerror(errno));
        exit(result);
    }
}

int startContainer(void *arg) {
    changeRootDirectory();
    changeWorkingDirectoryToRoot();
    mountProcFilesystem();
    startShell();

    return 0;
}

int main() {
    printf("Starting a container...\n\n");

    int stackSizeBytes = 65536;
    char *stackStart = malloc(stackSizeBytes);
    char *stackEnd = stackStart + stackSizeBytes;

    int flags = CLONE_NEWPID | SIGCHLD;
    pid_t childPid =
        clone(
            startContainer,
            stackEnd,
            flags,
            NULL
        );

    if (childPid == -1) {
        printf("clone failed: %s", strerror(errno));
    }

    waitpid(childPid, NULL, 0);

    umount("./container_root/proc");

    printf("\nContainer terminated\n");

    return 0;
}

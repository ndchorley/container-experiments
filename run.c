#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
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

void setHostname() {
    const char * hostname = "container";
    int result = sethostname(hostname, strlen(hostname));

    if (result == -1) {
        printf("Setting hostname failed: %s", strerror(errno));
        exit(result);
    }
}

int startContainer(void *arg) {
    changeRootDirectory();
    changeWorkingDirectoryToRoot();
    mountProcFilesystem();
    setHostname();
    startShell();

    return 0;
}

void createCgroupWithMemoryLimit() {
    int result = mkdir(
        "/sys/fs/cgroup/container",
        S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH
    );

    if (result == -1) {
        printf("Creating cgroup failed: %s", strerror(errno));
        exit(result);
    }

    FILE* memoryMaxFile =
        fopen("/sys/fs/cgroup/container/memory.max", "w");

    fprintf(memoryMaxFile, "%d\n", 100 * 1024 * 1024);
    fclose(memoryMaxFile);
}

void addChildToCgroup(pid_t pid) {
    FILE* procsFile =
        fopen("/sys/fs/cgroup/container/cgroup.procs", "w");
    fprintf(procsFile, "%d\n", pid);
    fclose(procsFile);
}

int main() {
    printf("Starting a container...\n\n");

    int stackSizeBytes = 65536;
    char *stackStart = malloc(stackSizeBytes);
    char *stackEnd = stackStart + stackSizeBytes;

    int flags =
        CLONE_NEWUTS |
        CLONE_NEWNS |
        CLONE_NEWUSER |
        CLONE_NEWPID |
        SIGCHLD;
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

    createCgroupWithMemoryLimit();
    addChildToCgroup(childPid);

    waitpid(childPid, NULL, 0);

    umount("./container_root/proc");

    printf("\nContainer terminated\n");

    return 0;
}

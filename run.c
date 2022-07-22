#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Running a shell...\n");

    char *arguments[] = {"bash", NULL};
    char *environment[] = {NULL};
    execve("/bin/bash", arguments, environment);

    printf("After execve - something went wrong\n");

    return 0;
}

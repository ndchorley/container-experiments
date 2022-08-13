#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf(
            "Usage: %s <amount of memory in megabytes>\n",
            argv[0]
        );

        exit(1);
    }

    size_t amountInMegabytes = atoi(argv[1]);

    void* start = malloc(amountInMegabytes * 1024 * 1024);

    if (start == NULL) {
        printf(
            "Unable to allocate %d megabytes of memory\n",
            amountInMegabytes
        );

        exit(1);
    }

    printf("Press any key to quit\n");
    getchar();

    free(start);

    return 0;
}

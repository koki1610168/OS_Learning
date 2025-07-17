#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        int what = wait(NULL);
        printf("Hello %d\n", what);
    } else {
        int rc_wait = wait(NULL);
        printf("Parent (pid: %d), (rc_wait: %d)\n", (int) getpid(), rc_wait);

    }


    return 0;
}

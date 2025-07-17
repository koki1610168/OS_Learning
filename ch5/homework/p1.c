#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int x = 100;
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        x = 50;
        printf("CHILD (pid:%d): x value is %d\n", (int) getpid(), x);
    } else {
        x = 400;

        int rc_wait = wait(NULL);
        printf("PARENT (pid:%d): x value is %d\n", (int) getpid(), x);
    }


    return 0;
}

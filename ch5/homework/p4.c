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
        char *my_args[2];
        my_args[0] = "/bin/ls";
        my_args[1] = NULL;
        execvp(my_args[0], my_args);
    } else {
        int rc_wait = wait(NULL);
    }


    return 0;
}

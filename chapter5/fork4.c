#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        char *args[] = {"pwd", NULL};
        execvp("pwd", args);
        perror("failed");
        exit(1);

        

    } else { // parent process
        wait(NULL);
        printf("Child finished\n");

        

    }
}

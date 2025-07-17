#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Hello\n");
        close(fd[0]);
        write(fd[1], "c", 1);
        close(fd[1]);
    } else {
        close(fd[1]);
        char c;
        read(fd[0], &c, 1);
        close(fd[0]); 
        printf("Good Bye\n");
    }


    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t p1 = fork();

    if (p1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    if (p1 == 0) { // child process
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", NULL);
        perror("execlp failed");
        exit(1);
    } 

    
    pid_t p2 = fork();

    if (p2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    if (p2 == 0) { // child process
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        execlp("sort", "sort", NULL);
        perror("execlp failed");
        exit(1);
    } 

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);
    return 0;


}

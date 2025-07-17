#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    pid_t child1, child2;
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "pipe creation failed\n");
        exit(1);
    }

    child1 = fork();

    if (child1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (child1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        char *my_args[3];
        my_args[0] = "echo";
        my_args[1] = "Hello from Koki";
        my_args[2] = NULL;

        execvp(my_args[0], my_args);
        
    }

    child2 = fork();

    if (child2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (child2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        execlp("wc", "wc", "-w", NULL);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);


    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    pid_t rc = fork();
    int status;

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
//        close(STDOUT_FILENO);
        printf("what the hell\n");
    } else {
        pid_t rc_wait = waitpid(rc, &status, 0);
        printf("Parent (pid: %d), (rc_wait: %d)\n", (int) getpid(), rc_wait);

        if (WIFEXITED(status)) {
            printf("normal\n");
        } else if (WIFSIGNALED(status)) {
            printf("signal\n");
        } else if (WIFSTOPPED(status)) {
            printf("stopped signal\n");
        }
            

    }


    return 0;
}

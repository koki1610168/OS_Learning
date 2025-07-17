#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd = open("./p2.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        char *text = "Hello\n";
        write(fd, text, strlen(text));
    } else {

        int rc_wait = wait(NULL);
        char *text = "Good Bye\n";
        write(fd, text, strlen(text));
    }


    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        fprintf(stderr, "file open failed\n");
    }

    int rc = fork();
    char* msg;

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        msg = "Child process is writing ... \n";
        write(fd, msg, strlen(msg)); 
        

    } else { // parent process
        //wait(NULL);
        msg = "parent process is writing ... \n";
        write(fd, msg, strlen(msg)); 
        

    }
}

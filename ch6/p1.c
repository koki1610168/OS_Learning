#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int fd = open("./p1.output", O_CREAT|O_RDONLY, S_IRWXU);
    int nloops = 100000;

    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < nloops; ++i) {
        read(fd, NULL, 0);
    }
    gettimeofday(&end, NULL);
    printf("%f microseconds\n", (float) (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / nloops); 

    close(fd);
    return 0;


}

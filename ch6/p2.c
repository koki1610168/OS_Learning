#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int nloops = 100000;
    struct timeval start, end;
    size_t cpusetsize = sizeof(cpu_set_t);
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(sched_getcpu(), &cpuset);
    if (sched_setaffinity(0, cpusetsize, &cpuset) == -1) {
        perror("sched affnity");
        exit(1);
    }

    int first_pipefd[2], second_pipefd[2];
    if (pipe(first_pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }
    if (pipe(second_pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t child = fork();
    if (child < 0) {
        perror("fork failed");
        exit(1);
    } else if (child == 0) {
        close(first_pipefd[1]);
        close(second_pipefd[0]);
        char buf = '0';
        for (int i = 0; i < nloops; ++i) {
            if (read(first_pipefd[0], &buf, 1) == -1) {
                perror("read failed");
                exit(1);
            }
            if (write(second_pipefd[1], &buf, 1) == -1) {
                perror("write failed");
                exit(1);
            }
        }


    } else {
        gettimeofday(&start, NULL);
        close(first_pipefd[0]);
        close(second_pipefd[1]);
        char buf = '0';
        for (int i = 0; i < nloops; ++i) {
            if (write(first_pipefd[1], &buf, 1) == -1) {
                perror("write failed");
                exit(1);
            }
            if (read(second_pipefd[0], &buf, 1) == -1) {
                perror("read failed");
                exit(1);
            }
        }
        gettimeofday(&end, NULL);

        wait(NULL);
        close(first_pipefd[1]);
        close(second_pipefd[0]);

        printf("%f microseconds\n", (float) (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / nloops);

    }
    return 0;


}

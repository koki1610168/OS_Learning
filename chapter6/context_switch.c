#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/wait.h>
#include <stdint.h>
#include <string.h>

#define NUM_ITERATIONS 1000000

long long time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

void bind_to_cpu(int cpu) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

int main() {
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        bind_to_cpu(0);

        char buf;
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            if (read(pipe1[0], &buf, 1) != 1) {
                perror("child read");
                exit(1);
            }
            if (write(pipe2[1], "x", 1) != 1) {
                perror("child write");
                exit(1);
            }
        }
        exit(0);
    } else {
        bind_to_cpu(0);

        char buf;
        struct timespec start, end;

        sleep(1);
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            if (write(pipe1[1], "x", 1) != 1) {
                perror("parent write");
                exit(1);
            }
            if (read(pipe2[0], &buf, 1) != 1) {
                perror("parent read");
                exit(1);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        long long total_ns = time_diff_ns(start, end);
        double avg_ctx_switch_ns = (double)total_ns / (NUM_ITERATIONS * 2);

        printf("Total time for %d ping-poing iterations: %lld ns\n", NUM_ITERATIONS, total_ns);
        printf("Average context switch time: %.2f ns \n", avg_ctx_switch_ns);

        wait(NULL);
    }

        return 0;
}

#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int getAffinity(pid_t pid, cpu_set_t cpuSet){
    char *state;

    int nCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU_ZERO(&cpuSet);
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }

    printf("CPU affinity:\n");
    for (int i = 0; i < nCPU; i++) {
        if (CPU_ISSET(i, &cpuSet)) state = "true";
        else state = "false";
        printf("\t CPU %d: %s\n", i, state);
    }

    return 0;
}

int setAffinity(pid_t pid, int numCPU, cpu_set_t cpuSet){
    printf("Set affinity for the CPU %d\n", numCPU);
    CPU_ZERO(&cpuSet);
    CPU_SET(numCPU, &cpuSet);
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_setaffinity");
        exit(-1);
    }

    return 0;
}

int main(int argc,char** argv){
    cpu_set_t cpuSet;
    int numCPU;

    if(argc<2 || sscanf(argv[1],"%d",&numCPU)!=1){
        printf("usage: %s numCPU\n",argv[0]);
        return(EXIT_FAILURE);
    }

    getAffinity(getpid(), cpuSet);
    printf("Actual CPU: %d\n\n", sched_getcpu());
    setAffinity(getpid(), numCPU, cpuSet);
    getAffinity(getpid(), cpuSet);
    printf("Actual CPU: %d\n\n", sched_getcpu());

    return 0;
}
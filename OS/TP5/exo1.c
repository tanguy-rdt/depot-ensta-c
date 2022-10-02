#define _GNU_SOURCE

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv){
    int previousCPU = sched_getcpu();
    printf("%d\n", previousCPU);

    while(1){
        if(previousCPU != sched_getcpu()){
            previousCPU = sched_getcpu();
            printf("%d\n", previousCPU);
        }
    }
}
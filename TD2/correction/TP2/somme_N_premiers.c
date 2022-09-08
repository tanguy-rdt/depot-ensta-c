#include <stdio.h>
#define N 100
int main(int argc, char* argv[]){
     int sum = 0;
    for (int i = 1; i <= N; i++) {
        sum += i;
        }
        printf("Theory : %d, find --> %d\n", N*(N+1)/2, sum);
    return 0;
}

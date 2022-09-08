#include <stdio.h>
#define N 10
int main(int argc, char* argv[]){
    int a[N];
    int b[N];

    // on remplit a avec des valeurs
    for (int i = 0; i < N; i++)
        a[i] = i*i;

    // on recopie a dans b
    for (int i = 0; i < N; i++)
        b[i] = a[i] ;

    // parcours simultané de a et b
    for (int i = 0; i < N; i++)
        printf("%d - a[%d] = %d - b[%d] = %d\n", i, i, a[i], i,  b[i]);

    // attention cela ne marche que si le tableau "a" est une variable locale !
    int n = sizeof(a)/sizeof(a[0]);
    printf("Raw array size ->  %ld, Size of int = %ld, Length of array = %d\n", sizeof(a), sizeof(a[0]), n);
    return 0;
}

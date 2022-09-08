#include <stdio.h>

int main(int argc, char* argv[]){
    int a = 3;
    int b = 5;
    int c = 1;
    
    // avec scanf
    printf("Enter the first value:");
    scanf("%d", &a);
    printf("Enter the second value:");
    scanf("%d", &b);
    printf("Enter the third value:");
    scanf("%d", &c);


    if (a > b + c || b > c + a || c > a + b){
        printf("%d %d %d n'est pas un triangle\n", a, b, c);
    } else {
        printf("%d %d %d est un triangle\n", a, b, c);
    }

    return 0;
}

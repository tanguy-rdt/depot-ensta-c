//
// Created by Tanguy Roudaut on 28/06/2022.
//

#include <stdio.h>

/*
1. int, float, long, short, char
2. int %d, float %f, long %ld, short %hd, char %s
3. bool
*/

float a = 10, b = 3, c = 0;
int e, f, g, x, y, z;

int main(){
    printf("Hello world !\n");
    printf("Je suis le deuxième programme\n");

    c = a / b;
    printf("%f divisée par %f, vaut %f", a, b, c);

    e = 4 - 3 * 2 - 1;
    f = 2 + 3 * 6 + 7 * 2 - (2/4);
    g = 8/4 + 2 * 10%5 + 3 + 2/1;
    printf("%d", e);
    printf("%d", f);
    printf("%d", g);

    z = 10 - 2;
    y = 4 * 2;
    x = z < y++;
    printf("%d", x);

    x = 3 > 2 > 1;
    y = x++ + 1;
    z = x||!y;
    printf("%d", z);

    return 0;
}
//
// Created by Tanguy Roudaut on 29/06/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rectangle(int, int);
void triangle(int);
void rectangle_pt(int, int);

int main(){
    // Partie 2
    /*int tab[]= {1, 2, 3, 4, 5, 6};

    int sum = 0;
    for(int i = 0; i < 6; i++){
        sum = sum + tab[i];
    }
    printf("La somme de tab est %d\n", sum);

    int tab1[] = {1, -2, -3, 4, -5, 6};
    sum = 0;
    for(int i = 0; i < 6; i++){
        if (tab1[i] >= 0){
            sum = sum + tab1[i];
        }
    }
    printf("La somme de tab est %d\n", sum);

    int tab2[] = {1, -2, -3, 4, -5, 6};
    sum = 0;
    for(int i = 0; i < 6; i++){
        if (tab2[i] >= 0){
            sum = sum + tab1[i];
        }
        else {
            break;
        }
    }
    printf("La somme de tab est %d\n", sum);

    char tab3 [] = "hello";
    int lenTab3 = strlen(tab3);
    char tab4[lenTab3];

    for (int i = 1; i < lenTab3+1; i++){
        tab4[i] = tab3[lenTab3-i];
    }

    printf("Le tableau initial est %s, le second est %s", tab3, tab4);*/

    // Partie 3
    rectangle(10, 4);
    printf("\n\n");
    triangle(11);
    //rectangle_pt(10, 4);

    // Partie 4
    char tab[] = {'u', 'n', '_', 'p', 'o', 'i', 'n', 't', 'e', 'u', 'r', '\0'};
    char* pTab = tab;
    char** addr_pTab = &pTab;
    printf("le tableau %s est a l'adresse %d\n", pTab, addr_pTab);

    int cnt=0;
    while(*(pTab+cnt) != '\0'){
        cnt++;
    }
    printf("La long du pointeur est de %d\n", cnt);

    char *pTabBis = &(*(pTab+9));
    *(pTab + 8) = '\0';

    printf("La chaine après coupure: %s\n", pTab);
    printf("La chaine coupé: %s", pTabBis);
}


void rectangle_pt(int a, int b){
    char tab[a][b];
    char *pTab = &tab[a][b];

    int len = a * b;
    for (int l = 0; l < len; l++){
        if (((l >= 0) && (l <= a)) || ((l >= (len - a)) && (l <= len))){
            *(pTab + l) = '*';
        }
        else if ((l%(len-1) == 0) || (l%len) == 0){
            *(pTab + l) = '*';
        }
        else {
            *(pTab + l) = ' ';
        }
    }

    printf("%s", pTab);
}


void rectangle(int a, int b){
    char tab[a][b];

    for(int y = 0; y < b; y++){
        if ((y == 0) || (y == b-1)) {
            for (int x = 0; x < a; x++) {
                tab[x][y] = '*';
            }
        }
        else {
            tab[0][y] = '*';
            for (int x = 1; x < a - 1; x++) {
                tab[x][y] = ' ';
            }
            tab[a-1][y] = '*';
        }
    }

    for(int y = 0; y < b; y++){
        for(int x = 0; x < a; x++){
            printf("%c", tab[x][y]);
        }
        printf("\n");
    }
}

void triangle(int a){
    char tab[a][a];

    for (int y = 0; y < a; y++){
        for (int x = 0; x < a; x++){
            tab[x][y] = ' ';
        }
    }

    int j = 0;
    for (int y = 0; y < a; y++){
        for (int x = j; x < a - j;x++){
            if (y == 0){
                tab[x][y] = '*';
            }
            else if ((x == j) || (x == (a-1)-j)){
                tab[x][y] = '*';
            }
            else {
                tab[x][y] = ' ';
            }
        }
        j++;
    }

    for (int y = 0; y < a; y++){
        for (int x = 0; x < a;x++){
            printf("%c", tab[x][y]);
        }
        printf("\n");
    }
}
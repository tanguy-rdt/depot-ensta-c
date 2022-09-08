//
// Created by Tanguy Roudaut on 28/06/2022.
//

#include <stdio.h>


/*
1. int, float, long, short, char
2. int %d, float %f, long %ld, short %hd, char %s
3. bool
*/

//Sans les pointeurs
/*void copieTabFor();
void copieTabWhile();

int main(){
    // partie 2
    int N = 10;
    int sum = 0;

    for (int i = 0; i <= N; i++){
        sum = i + sum;
    }
    printf("La somme est de %d\n\n\n", sum);


    char tab[] = "Bonjourra";
    bool rInTab = false;
    int cnt = 0;

    for (int i = 0; i < 10; i++){
        if ((tab[i] == 'r') || rInTab == true){
            rInTab = true;

            if (tab[i] == 'r') {
                cnt += 1;
            }
        }
    }
    if (rInTab) {
        printf("r est dans %s\n", tab);
        printf("Il y a %d r dans %s\n\n\n", cnt, tab);
    }

    copieTabFor();

    // Partie 3
    copieTabWhile();

    // Partie 4

    return 0;
}


void copieTabFor(){
    char a[] = "abcdefghi";
    char b[10];

    printf("le tab a vaut %s et le tab b vaut %s\n", a, b);
    for (int i = 0; i < 10; i++){
        b[i] = a[i];
    }
    printf("le tab a vaut %s et le tab b vaut %s grâce au for\n\n\n", a, b);
}

void copieTabWhile(){
    char c[] = "abcdefghi";
    char d[10];

    printf("le tab a vaut %s et le tab b vaut %s\n", c, d);
    int i = 0;
    while(i != 10){
        d[i] = c[i];
        i++;
    }
    printf("le tab a vaut %s et le tab b vaut %s grâce au while\n", c, d);

    bool tabCopied = true;
    while ((tabCopied) && (i < 10)) {
        if (d[i] != c[i]){
            tabCopied = false;
        }
    }
    if (tabCopied) {
        printf("Le tableau est similaire: %d\n\n", tabCopied);
    }
}*/


void copieTabFor();
void copieTabWhile();

int main(){
    // partie 2
    int N = 10;
    int sum = 0;

    for (int i = 0; i <= N; i++){
        sum = i + sum;
    }
    printf("La somme est de %d\n\n\n", sum);


    char tab[] = "bjojihrgkjd";
    char *pTab = &tab[0];

    bool rInTab = false;
    int cnt = 0;

    for (int i = 0; i < 10; i++){
            if (*(pTab + i) == 'r') {
                cnt += 1;
            }
    }
    if (cnt >= 1) {
        printf("r est dans %s\n", pTab);
        printf("Il y a %d r dans %s\n\n\n", cnt, pTab);
    }

    copieTabFor();

    // Partie 3
    copieTabWhile();

    // Partie 4

    return 0;
}


void copieTabFor(){
    char a[] = "abcdefghi";
    char b[10];
    char* pA = a;
    char* pB = b;

    printf("le tab a vaut %s et le tab b vaut %s\n", pA, pB);
    for (int i = 0; i < 10; i++){
        *(pB+i) = *(pA+i);
    }
    printf("le tab a vaut %s et le tab b vaut %s grâce au for\n\n\n", pA, pB);
}

void copieTabWhile(){
    char c[] = "abcdefghi";
    char d[10];
    char* pC = &c[0];
    char* pD = &d[0];

    printf("le tab a vaut %s et le tab b vaut %s\n", pC, pD);
    int i = 0;
    while(i != 10){
        *(pD + i) = *(pC + i);
        i++;
    }
    printf("le tab a vaut %s et le tab b vaut %s grâce au while\n", pC, pD);

    bool tabCopied = true;
    while ((tabCopied) && (i < 10)) {
        if (*(pD + i) != *(pC + i)){
            tabCopied = false;
        }
    }
    if (tabCopied) {
        printf("Le tableau est similaire: %d\n\n", tabCopied);
    }
}




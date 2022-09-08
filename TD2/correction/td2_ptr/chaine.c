#include <stdio.h>

int main(int argc, char* argv[]){
    char tab[] = "un pointeur";
    long int len_tab;
    printf("adresse de tab %p\n", tab);

    char *ptab = tab;
    while(*ptab != '\0'){
        printf("%c", *ptab);
        ptab += 1; 
    }
    printf("\n");
    printf("la taille de la chaîne est %ld\n", ptab - tab); 
    len_tab = ptab - tab;
    printf("%ld\n", len_tab);

    *(tab + 8) = '\0'; // idem ligne suivante
    tab[8] = '\0';
    printf("contenu de tab %s\n", tab);
    printf("contenu de tab +9: %s\n", tab + 9);
    return 0;
}

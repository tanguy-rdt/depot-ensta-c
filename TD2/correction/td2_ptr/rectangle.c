#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    /*
    On choisit ici d'afficher un rectangle avec des lignes vides et pas remplies de *
    et de construire les chaînes à afficher au lieu de les refaire à la volée à chaque
    fois. Ce n'est pas la seule approche possible.
    */
    int nb_l = 5;
    int nb_c = 10;

    int* nb_l_ptr;
    nb_l_ptr = &nb_l;

    char col[nb_c + 1]; // +1 pour le \0
    char col_start_stop[nb_c +1];

    col[0] = col_start_stop[0] = '*';
    col[nb_c - 1] = col_start_stop[nb_c - 1] = '*';
    col[nb_c] = col_start_stop[nb_c] = '\0';

    for (int i = 1; i < nb_c - 1; i++){
        col[i] = ' ';
        col_start_stop[i] = '*';
    }

        
    *nb_l_ptr = 10;
    printf("%d %d %p %p\n", nb_l, *nb_l_ptr, nb_l_ptr, &nb_l);

    printf("%s\n", col_start_stop);
    for (int i = 1; i < *nb_l_ptr - 1; i++){
        printf("%s\n", col);
        system("sleep 1");
 }
    printf("%s\n", col_start_stop);
    return 0;
} 

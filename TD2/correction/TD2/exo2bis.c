#include <stdio.h>

int main(int argc, char* argv[]){
    char original[] = "hello"; // le \0 est ajouté implicitement 
    char original2[] = {'h', 'e', 'l', 'l', 'o', '\0'}; // ici il faut le spécifier

    /*
    If there are fewer initializers in a brace-enclosed list than there are
    elements or members of an aggregate, or fewer characters in a string literal
    used to initialize an array of known size than there are elements in the
    array, the remainder of the aggregate shall be initialized implicitly the
    same as objects that have static storage duration.
    */

    char reverse[6] = {'\0'}; // solution de facilité on initialise tout à \0
    char reverse2[6]; // ici on évite de faire des initialisations pour rien (peut être utile si gros tableau)

    // while (original[i] != '\0') marche aussi si chaîne bien formée mieux vaut contrôler la taille de i aussi
    for (int i = 0; i < 5; i++){
        reverse[4 - i] = original[i];
        reverse2[4 - i] = original[i];
    }

    // ne pas oublier
    reverse2[5] = '\0';

    printf("chaine de départ: %s\n", original);
    printf("chaine à l'envers: %s\n", reverse);
    printf("chaine à l'envers bis: %s\n", reverse2);

    return 0;
}


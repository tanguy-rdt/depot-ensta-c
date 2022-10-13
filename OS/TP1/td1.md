# TD1 - Les processus

## Exercice 1
#

### Question 1:
```c
# include ...
main () {
    printf (“Debut du test fork ()\n”);

    int code_retour = fork (); // Création du fils, ret = pidFils > 0 si c'est le père
                               //                   ret = 0 si c'est le fils
                               //                   ret = -1 si il y a un echec

    switch (code_retour ) {
        case –1 : // -1 donc un echec
            printf (“Pbm lors de la creation du processus\n”);
            break;
        case 0 : // 0 donc le fils
            printf (“Je suis le processus fils \n”);
            break;  
        default : // > 0, donc le pid du fils, c'est le pére
            printf (“Je suis le processus père\n”);
            printf (“Je viens de créer le processus fils dont le pid est %d \n”,code_retour);
    }

    printf (“code_retour %d\n”, code_retour);
    printf (“Fin du test fork ()\n”);
}
```
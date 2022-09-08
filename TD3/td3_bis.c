#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

// il n'est pas obligatoire de nommer les arguments
// dans les entêtes

typedef struct  _args {
    long val;
    struct _args* next;
} args_t;

long average_long_array(int array_size, args_t* ptrList);
int fill_tab(int argc, char* argv[], args_t** ptrList);

int main(int argc, char *argv[]){
    args_t* ptrList;

    int nb_args_read;
    nb_args_read = fill_tab(argc, argv, &ptrList);

    long tab_mean = average_long_array(nb_args_read, ptrList);
    printf("la moyenne vaut %ld\n", tab_mean);


    return 0;
}

int fill_tab(int argc, char* argv[], args_t** ptrList){
    int cpt_arg = 0;
    *ptrList = NULL; // histoire de pas envoyer un truc vide
    args_t* ptrCurrentArg;

    args_t maList;

    if (argc == 1){
        printf("Usage: %s nb1 nb2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* endptr;
    for(int i=0; i < argc - 1; i++){
        long tmp;
        errno = 0;
        tmp = strtol(argv[i + 1], &endptr, 10);
        if ((errno == ERANGE && (tmp == LONG_MAX || tmp == LONG_MIN))
            || (errno != 0 && tmp == 0)) {
            perror("strtol");
            exit(EXIT_FAILURE);
        }

        if (endptr == argv[i + 1]) {
            fprintf(stderr, "Pas de chiffre trouvé en %dème position\n", i + 1);
            continue; // on arrête la pour cette arg et on passe au suivant
        }
        // on est sûr d'avoir un argument exploitable
        cpt_arg += 1;

        if (!i){
            *ptrList = malloc(sizeof (args_t));
            ptrCurrentArg = *ptrList;
            ptrCurrentArg -> val = tmp;
            ptrCurrentArg -> next = NULL;
        }
        else{
            ptrCurrentArg -> next = (args_t *)malloc(sizeof(args_t));
            ptrCurrentArg = ptrCurrentArg -> next;
            ptrCurrentArg -> val = tmp;
            ptrCurrentArg -> next = NULL;
        }






    }
    return cpt_arg;
}



long average_long_array(int array_size, args_t* ptrList){
    long sum = 0;

    while(ptrList){
        sum += ptrList -> val;
        ptrList = ptrList -> next;
    }

    return sum / array_size;
}
//
// Created by Tanguy Roudaut on 18/09/2022.
//
#include <sys/types.h>     /* key_t  */
#include <sys/ipc.h>       /* ftok   */
#include <sys/sem.h>       /* semget, semctl, semop */
#include <sys/stat.h>      /* stat, struct stat  */
#include <stdlib.h>        /* malloc */
#include <stdio.h>         /* perror, printf */
#include <errno.h>         /* errno */
#include <unistd.h> 	   /*sleep*/

#define PRJVAL  1
#define MUTEX_FILE 0 // macro pour la lecture du fichier
#define MUTEX_L 1 // macro pour le processus lecteur


/** fonction pour mettre a jour le nb de lecteur **/
int update_nb_lecteur(int val){
    int state;

    FILE *fp;
    fp = fopen("nombre", "w"); // ouverture/création du fichier, le contenu est effacé si le fichier existe
    state = fprintf(fp, "%d", val); // écriture du nombre de lecteur actif dans le fichier
    fclose(fp); // fermeture du fichier

    if (state < 0){
        printf("ERROR: Nothing was written\n");
    }
    return state;
}

/** fonction pour obtenir le nb de lecteur actif **/
int get_nb_lecteur(){
    int nb;
    int state;

    FILE *fp;
    fp = fopen("nombre", "r"); // ouverture du fichier pour lire le conteu, le fichier doit existé
    state = fscanf(fp, "%d", &nb); // on récupére le nombre de lecteur et on le stock dans la var nb
    fclose(fp); // fermeture du fichier

    if (state == 0){
        printf("ERROR: Nothing to read\n");
    }
    return nb; // on retourne le nombre de fichier
}

/** fonction pour lire le fichier **/
int lire(const char *filename){
    char str1[6], str2[6], str3[2], str4[4], str5[4], str6[9]; // var pour stocker le contenu
    int date, year; // var pour stocker le contenu
    int state;

    FILE *fp;
    fp = fopen(filename, "r"); // ouverture du fichier pour lire le conteu, le fichier doit existé
    state = fscanf(fp, "%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, &date, str6, &year); // on lit le contenu
    printf("%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, date, str6, year); // on affiche le contenu
    fclose(fp); // on ferme le fichier

    if (state == 0){
        printf("ERROR: Nothing to read\n");
    }
    return state;
}

int P(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    Ops[0].sem_num = noSem; // numéro du sem à affecter l'opération
    Ops[0].sem_op = -1; // Opération -- comme P()
    Ops[0].sem_flg = 0;

    // arg1: l'id de l'ensemble des sémaphores, arg2: opération à effectuer, arg3: nombre d'opération à effectuer
    if((ok = semop(semid, Ops, 1)) == -1){// Réalisation de l'opération
        printf("Err %d P()", ok);
        exit(-1); // err retour -1
    }
    return ok; // return 0
}

int V(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    Ops[0].sem_num = noSem; // numéro du sem à affecter l'opération
    Ops[0].sem_op = +1; // Opération ++ comme V()
    Ops[0].sem_flg = 0;

    // arg1: l'id de l'ensemble des sémaphores, arg2: opération à effectuer, arg3: nombre d'opération à effectuer
    if((ok = semop(semid, Ops, 1)) == -1){// Réalisation de l'opération
        printf("Err %d V()", ok);
        exit(-1); // err retour -1
    }
    return ok; // return 0
}

int main (void)
{
    const char *filename = "myFile.txt";
    int nb_lecteur;

    key_t k;
    k = ftok("myKey", PRJVAL); // On récupère la même clé que celle utilisée pour la création des sem

    int semid = semget(k, 0, 0); // On ouvre l'esemble des sem avec la clé et on recup l'id
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);

    P(semid, MUTEX_L); // On prend le lecteur pour bloquer la modification du nombre de lecteur
    nb_lecteur = get_nb_lecteur(); // on récupère le nb de lecteur
    update_nb_lecteur(nb_lecteur+1); // on incrémente le nombre de lecteur
    if(get_nb_lecteur() == 1){
        printf("%d", P(semid, MUTEX_FILE)); // si c'est le premier lecteur alors on prend le redacteur pour eviter une modif
    }
    V(semid, MUTEX_L); // on libére le lecteur

    sleep(5); // sleep bidon
    lire(filename); // on lit le fichier

    P(semid, MUTEX_L); // On prend le lecteur pour bloquer la modification du nombre de lecteur
    nb_lecteur = get_nb_lecteur(); // on récupère le nb de lecteur
    update_nb_lecteur(nb_lecteur-1); // on décrémente le nombre de lecteur
    if(get_nb_lecteur() == 0){
        V(semid, MUTEX_FILE); // si c'est le dernier lecteur on libére le redacteur pour une modif potentiel
    }
    V(semid, MUTEX_L);  // on libére le lecteur

    //semctl(semid, 2, IPC_RMID); // supp des sem
    return 0;
}
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
#include <time.h>

#define PRJVAL  1
#define MUTEX_FILE 0 // macro pour la lecture du fichier
#define MUTEX_L 1 // macro pour le processus lecteur

time_t myTime;

/** fonction pour l'écriture dans un fichier **/
int ecrire(const char *filename){
    int state;
    time(&myTime); // en plus pour écrire l'heur dans le fichier

    FILE *fp;
    fp = fopen(filename, "w"); // ouverture/création du fichier, le contenu est effacé si le fichier existe
    state = fprintf(fp, "%s %s", "Hello World -", ctime(&myTime)); // écriture dans le fichier
    fclose(fp); // fermeture du fichier

    if (state < 0){
        printf("ERROR: Nothing was written\n");
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
    int stateMutexR;

    key_t k;
    k = ftok("myKey", PRJVAL); // On récupère la même clé que celle utilisée pour la création des sem

    int semid = semget(k, 0, 0); // On ouvre l'esemble des sem avec la clé et on recup l'id
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);

    P(semid, MUTEX_FILE); // On prend le rédacteur pour bloquer les lecteurs
    sleep(10); // sleep bidon
    ecrire(filename); // on écris dans le fichier
    V(semid, MUTEX_FILE); // On libére le R pour permettre au L de lire le fichier

    //semctl(semid, 2, IPC_RMID); // suppression des sem
    return 0;
}
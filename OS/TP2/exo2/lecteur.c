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
//#define MUTEX_R 0
//#define MUTEX_L 1



int update_nb_lecteur(int val){
    FILE *fp;
    fp = fopen("nombre", "w+");
    fprintf(fp, "%d", val);
    fclose(fp);

    return 0;
}

int get_nb_lecteur(){
    int nb;
    FILE *fp;
    fp = fopen("nombre", "r+");
    fscanf(fp, "%d", &nb);
    fclose(fp);

    return nb;
}

int lire(const char *filename){
    char str1[6], str2[6], str3[2], str4[4], str5[4], str6[9];
    int date, year;

    FILE *fp;
    fp = fopen(filename, "r");
    fscanf(fp, "%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, &date, str6, &year);
    printf("%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, date, str6, year);
    fclose(fp);

    return 0;
}

int P(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    Ops[0].sem_num = noSem;
    Ops[0].sem_op = -1;
    Ops[0].sem_flg = 0;

    ok = semop(semid, Ops, 1);
    return ok;
}

int V(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    Ops[0].sem_num = noSem;
    Ops[0].sem_op = +1;
    Ops[0].sem_flg = 0;

    ok = semop(semid, Ops, 1);
    return ok;
}

int main (void)
{
    const char *filename = "myFile.txt";

    int nb_lecteur;
    int MUTEX_R = 0;
    int MUTEX_L = 1;

    key_t k;
    k = ftok("myKey", PRJVAL);

    int semid = semget(k, 0, 0);
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);

    P(semid, MUTEX_L);
    nb_lecteur = get_nb_lecteur();
    update_nb_lecteur(nb_lecteur+1);
    if(get_nb_lecteur() == 1){
        printf("%d", P(semid, MUTEX_R));
    }
    V(semid, MUTEX_L);

    sleep(5);
    lire(filename);

    P(semid, MUTEX_L);
    nb_lecteur = get_nb_lecteur();
    update_nb_lecteur(nb_lecteur-1);
    if(get_nb_lecteur() == 0){
        V(semid, MUTEX_R);
    }
    V(semid, MUTEX_L);

    //semctl(semid, 2, IPC_RMID);
    return 0;
}
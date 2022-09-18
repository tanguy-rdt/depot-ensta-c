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
//#define MUTEX_R 0
//#define MUTEX_L 1

time_t myTime;

int ecrire(const char *filename){
    time(&myTime);

    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%s %s", "Hello World -", ctime(&myTime));
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
    int MUTEX_R = 0;
    int MUTEX_L = 1;

    key_t k;
    k = ftok("myKey", PRJVAL);

    int semid = semget(k, 0, 0);
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);

    P(semid, MUTEX_R);
    sleep(10);
    ecrire(filename);
    V(semid, MUTEX_R);

    //semctl(semid, 2, IPC_RMID);
    return 0;
}
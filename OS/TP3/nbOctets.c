/* Example using raise by TechOnTheNet.com */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "partage.h"


void comHandler(int sig)
{
    if (sig == SIGUSR1) printf("[%d] -- Reception du signal SIGUSR1\n", getpid());
}

int main(int argc, const char * argv[])
{

    printf("[%d] -- PID du pere\n", getpid());

    pid_t pidWC;
    pid_t pidREAD;

    int statusWaitPidWC, statusPidWC;
    int statusWaitPidREAD, statusPidREAD;

    FILE *fIn;
    char *fileName=NULL;

    struct sigaction action;

    if (argc!=2)
    {
        fprintf(stderr,"Usage: %s fileName\n",argv[0]);
        return 1;
    }
    fileName=argv[1];

    /* Gestion des signaux */
    /* =================== */
    action.sa_handler = &comHandler;
    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    else {
        printf("[%d] -- Communication handler set\n", getpid());
    }

    /* Creation de la zone de memoire partagee */
    /* ======================================= */
    Zone z;
    if (creerZonePartagee(sizeof(int),&z)==-1) exit(-1);
    int *ptDeb=(int*)z.debut;

    /* Creation du tube */
    /* ================ */
    printf("[%d] -- Creation du tube\n", getpid());
    int tube[2];
    pipe(tube);

    /* Creation du processus qui fera le exec ... */
    /* ============================================ */
    printf("[%d] -- Creation du fils pidWC\n", getpid());
    pidWC=fork();
    if(pidWC == 0){
        printf("[%d] -- PID du fils pidWC\n", getpid());
        close(tube[0]);
        close(1);
        dup(tube[1]);
        close(tube[1]);
        execl("/usr/bin/wc", "wc", "-c", fileName, NULL);
        exit(0);
    }

    /* Creation du processus qui fera la lecture ...*/
    /* ============================================ */
    printf("[%d] -- Creation du fils pidREAD\n", getpid());
    pidREAD=fork();
    if (pidREAD == 0){
        printf("[%d] -- PID du pidREAD\n", getpid());
        close(tube[1]); // fermeture en écriture
        close(0);
        fIn=fdopen(tube[0],"r");
        int nbChar;
        fscanf(fIn, "%d", &nbChar);
        ptDeb = &nbChar;
        printf("[%d] -- Le fils pidREAD lis dans la mp: %d\n", getpid(), *ptDeb);
        fclose(fIn);
        printf("[%d] -- Sleep 5sec\n", getpid());
        sleep(5);
        printf("[%d] -- Envoie sig SIGUSR1\n", getpid());
        sigqueue(getppid(), SIGUSR1, NULL);
        exit(0);
    }

    sleep(1);

    /* La suite du pere */
    /* ================ */
    /* Fermer les descripteurs de tube inutiles au pere */ //TODO
    /* ... */
    printf("[%d] -- Attente du sig SIGUSR1\n", getpid());
    pause();
    printf("[%d] -- Le pere lis dans la mp: %d\n", getpid(), *ptDeb);
    statusWaitPidWC = waitpid(pidWC, &statusPidWC, 0);
    statusWaitPidREAD = waitpid(pidREAD, &statusPidWC, 0);
    supprimerZonePartagee(&z);

    return 0;
}


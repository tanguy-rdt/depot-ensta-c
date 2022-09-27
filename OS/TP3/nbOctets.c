#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "partage.h"

/* Handler de signal pour SIGUSR1 */
/* ============================== */
void comHandler(int sig){
    if (sig == SIGUSR1) printf("[%d] -- (PERE) -- Reception du signal SIGUSR1\n", getpid());
}


/* Le main */
/* ======= */
int main(int argc, const char * argv[]){
    pid_t pidWC;
    pid_t pidREAD;

    int statusWaitPidWC, statusPidWC;
    int statusWaitPidREAD, statusPidREAD;

    FILE *fIn;
    char *fileName = NULL;

    struct sigaction action;

    if (argc!=2){
        fprintf(stderr,"Usage: %s fileName\n",argv[0]);
        return 1;
    }
    fileName = argv[1];


    /* Gestion des signaux */
    /* =================== */
    action.sa_handler = &comHandler;
    if (sigaction(SIGUSR1, &action, NULL) == -1){
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    else {
        printf("[%d] -- (PERE) -- Communication handler set\n", getpid());
    }


    /* Creation de la zone de memoire partagee */
    /* ======================================= */
    Zone z;
    if (creerZonePartagee(sizeof(int),&z)==-1) exit(-1);
    int *ptDeb=(int*)z.debut;


    /* Creation du tube */
    /* ================ */
    int tube[2];
    pipe(tube);


    /* Creation du processus qui fera le exec ... */
    /* ============================================ */
    pidWC=fork();
    if(pidWC == 0){
        printf("[%d] -- (FILS WC) -- Execution de la commande 'wc -c %s'\n", getpid(), fileName);
        close(tube[0]); // fermeture en lecture (sortie du tube)
        close(1); // pas d'écriture sur l'écran
        dup(tube[1]); // la sortie standard devient l'entrée du tube
        close(tube[1]); // descripteur inutile après redirection
        execl("/usr/bin/wc", "wc", "-c", fileName, NULL);
        exit(0);
    }
    else if(pidWC == -1){
        printf("WC, ERROR: -1");
        exit(-1);
    }


    /* Creation du processus qui fera la lecture ...*/
    /* ============================================ */
    pidREAD=fork();
    if (pidREAD == 0){
        close(tube[1]); // fermeture en écriture (entrée du tube)
        close(0); // pas de lecture sur le clavier
        fIn=fdopen(tube[0],"r"); // descripteur fIn est sur la sortie du tube (lecture)
        printf("[%d] -- (FILS READ) -- Lecture à la sortie du tube\n", getpid());
        fscanf(fIn, "%d", ptDeb); // on obtient la valeur en sortie du tube (retour de wc -c)
        printf("[%d] -- (FILS READ) -- Ecriture du nombre de caractères de %s (%d) dans la mémoire partagé\n", getpid(), fileName, *ptDeb);
        fclose(fIn);
        printf("[%d] -- (FILS READ) -- Sleep 5sec avant d'envoyer le signal\n", getpid());
        sleep(5);
        printf("[%d] -- (FILS READ) -- Envoie du signal SIGUSR1\n", getpid());
        kill(getppid(), SIGUSR1);
        exit(0); // on termine le processus
    }
    else if(pidREAD == -1){
        printf("READ, ERROR: -1");
        exit(-1);
    }


    /* La suite du pere */
    /* ================ */
    sleep(1);
    close(tube[1]); // fermeture en écriture (entrée du tube)
    close(tube[0]); // fermeture en lecture (sortie du tube)
    close(0); // pas de lecture sur le clavier
    printf("[%d] -- (PERE) -- Attente du signal SIGUSR1\n", getpid());
    pause();
    printf("[%d] -- (PERE) -- Lecture dans la mémoire partagé: %d\n", getpid(), *ptDeb);
    statusWaitPidWC = waitpid(pidWC, &statusPidWC, 0);
    statusWaitPidREAD = waitpid(pidREAD, &statusPidREAD, 0);
    supprimerZonePartagee(&z);
    printf("\n[%d] -- (PERE) -- Fin du TP, status pidWC: %d, status pidWC: %d\n", getpid(), statusPidWC, statusPidREAD);

    return 0;
}


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
    /* Display a message indicating we have received a signal */
    if (sig == SIGUSR1) printf("Received a SIGUSR1 signal\n");
}

int main(int argc, const char * argv[])
{

    pid_t pidWC;
    pid_t pidREAD;

    int statusWaitPidWC, statusPidWC;
    int statusWaitPidREAD, statusPidREAD;

    FILE *fIn; /* Pour faire un fdopen : int -> FILE * */
    int *ptDeb; /* Un pointeur (int*) sur la zone debut */
    char *fileName=NULL;

    struct sigaction action;

    if (argc!=2)
    {
        fprintf(stderr,"Usage: %s fileName\n",argv[0]);
        return 1;
    }
    fileName=argv[1];
    /* A cause de warnings lorsque le code n’est pas encore la ...*/
    (void)fIn; (void)pidREAD;
    (void)pidWC;

    /* Gestion des signaux */
    /* =================== */
    /* Preparation de la structure action pour recevoir le signal SIGUSR1*/
    action.sa_handler = &comHandler;
    /* Appel a l’appel systeme sigaction */
    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Communication handler set\n");
    }

    /* Creation de la zone de memoire partagee */
    /* ======================================= */
    Zone z;
    if (creerZonePartagee(sizeof(int),&z)==-1) exit(-1);
    ptDeb=(int*)z.debut;


    /* Creation du tube */
    /* ================ */
    printf("creation tube\n");
    int tube[2];
    pipe(tube);

    printf("creation pidWC\n");
    printf("pid du pere %d\n", getpid());

    /* Creation du processus qui fera le exec ... */
    /* ============================================ */
    pidWC=fork();
    if(pidWC == 0){
        printf("pid du pidWC %d\n", getpid());
        printf("enter pidWC\n");
        close(tube[0]); // fermeture en lecture
        close(1); // pas d'écriture sur l'écran (stdout)
        dup(tube[1]); // df[1] devient sortie standart
        close(tube[1]); // description inutile après redirection
        execl("/usr/bin/wc", "wc", "-c", fileName, NULL); // recouvrement par la commande 'wc -c' //TODO
        printf("exit pidREAD\n");
        exit(0);
    }
    printf("creation pidREAD\n");

    pidREAD=fork();
    if (pidREAD == 0){
        printf("pid du pidREAD %d\n", getpid());
        printf("enter pidREAD\n");
        close(tube[1]); // fermeture en écriture
        close(0);
        fIn=fdopen(tube[0],"r");
        int nbChar;
        fscanf(fIn, "%d", &nbChar);
        writeZonePartagee(&z, 10);
        printf("Lu dans la mp: %d\n", z.taille);
        printf("z.id: %d\n", z.id);
        fclose(fIn);
        sleep(5);
        printf("send sig\n");
        raise(SIGUSR1);
        printf("Lu dans la mp: %d\n", z.taille);
        printf("z.id: %d\n", z.id);
        exit(0);
    }

    sleep(1);

    /* La suite du pere */
    /* ================ */
    /* Fermer les descripteurs de tube inutiles au pere */
    /* ... */
    /* Attente d’un signal */
    printf("pid du att %d\n", getpid());

    printf("attente sig\n");
    pause();
    /* Recuperer le resultat dans la memoire partagee */
    int nbCharRead = z.taille;
    printf("Lu dans la mp: %d\n", nbCharRead);
    printf("z.id: %d\n", z.id);

    /* Attendre le 1er enfant */
    statusWaitPidWC = waitpid(pidWC, &statusPidWC, 0);
    /* Attendre le 2eme enfant */
    statusWaitPidREAD = waitpid(pidREAD, &statusPidWC, 0);
    /* Supprimer la memoire partagee */
    supprimerZonePartagee(&z);


    /* Display a message indicating we are leaving main */
    printf("Finished main\n");

    return 0;
}


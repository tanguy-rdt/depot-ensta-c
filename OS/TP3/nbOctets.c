//
// Created by Tanguy Roudaut on 21/09/2022.
//
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "partage.h"

/* Decrire le handler de signal pour SIGUSR1 */
/* ========================================= */
/* ... */
/* Le main */
/* ======= */
int main(int argc,char **argv)
{
    pid_t pidWC;
    pid_t pidREAD;
    int status; /* Pour les waitpid */
    int tube[2];
    FILE *fIn; /* Pour faire un fdopen : int -> FILE * */
    struct sigaction action;
    int *ptDeb; /* Un pointeur (int*) sur la zone debut */
    char *fileName=NULL;
    if (argc!=2)
    {
        fprintf(stderr,"Usage: %s fileName\n",argv[0]);
        return 1;
    }
    fileName=argv[1];
    /* A cause de warnings lorsque le code n’est pas encore la ...*/
    (void)action; (void)fIn; (void)tube; (void)status; (void)pidREAD;
    (void)pidWC;

    /* Gestion des signaux */
    /* =================== */
    /* Preparation de la structure action pour recevoir le signal SIGUSR1
    */
    /* action.sa_handler = ... */
    /* Appel a l’appel systeme sigaction */
    /* ... */


    /* Creation de la zone de memoire partagee */
    /* ======================================= */
    /* ... */
    Zone z;
    if (creerZonePartagee(sizeof(int),&z)==-1) exit(-1);


    ptDeb=(int*)z.debut; /* *ptDeb <=> *((int*)z.debut) */


    /* Creation du tube */
    /* ================ */
    int df[2];
    pipe(df);


    /* Creation du processus qui fera le exec ... */
    /* ============================================ */
    pidWC=fork();
    if(pidWC == 0){
        close(df[0]); // fermeture en lecture
        close(1); // pas d'écriture sur l'écran (stdout)
        dup(df[1]); // df[1] devient sortie standart
        close(df[1]) // description inutile après redirection
        execl("/usr/bin/wc", "wc", "-c", NULL); // recouvrement par la commande 'wc -c'
    }


    /* Creation du processus qui fera la lecture ...*/
    /* ============================================ */
    pidREAD=fork();
    if (pidREAD){
        close(df[1]); // fermeture en écriture
        close(0);
        dup(df[0]);
        close(df[0]);


    }

    /* Dans le processus cree :
    - Fermer le(s) descripteur(s) inutile(s) a cet enfant
    - Ouvrir un flux fIn sur la sortie du tube:
    fIn=fdopen(tube[0],"r");
    - Lire le resultat via le flux fIn et le mettre dans la memoire
    partagee
    - Fermer le flux fIn et le(s) descripteur(s) encore ouvert(s)
    - Attendre un peu pour que le pere puisse faire pause avant
    - Envoyer le signal SIGUSR1 au pere
    */


    /* La suite du pere */
    /* ================ */
    /* Fermer les descripteurs de tube inutiles au pere */
    /* ... */
    /* Attente d’un signal */
    /* ... */
    /* Recuperer le resultat dans la memoire partagee */
    /* ... */
    /* Attendre le 1er enfant */
    /* ... */
    /* Attendre le 2eme enfant */
    /* ... */
    /* Supprimer la memoire partagee */
    /* ... */

    return 0;
}


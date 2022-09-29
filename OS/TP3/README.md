---
title: "TP3 - Communication Inter-Processus"
date: 21 septembre 2022
---

# Table des matières

- [Table des matières](#table-des-matières)
- [La mémoire partagé](#la-mémoire-partagé)
  - [Les primitives de la mémoire partagé](#les-primitives-de-la-mémoire-partagé)
  - [Utilisation de la mémoire partagé](#utilisation-de-la-mémoire-partagé)
- [Les tubes](#les-tubes)
  - [Les fonctions des tubes](#les-fonctions-des-tubes)
  - [Utilisation des tubes](#utilisation-des-tubes)
- [Les signaux](#les-signaux)
  - [Les fonctions des signaux](#les-fonctions-des-signaux)
  - [Utilisation des signaux](#utilisation-des-signaux)
- [Le code du TP](#le-code-du-tp)


# La mémoire partagé
## Les primitives de la mémoire partagé
   1. **La primitive ```shmget()```** :

        Permet la création d'un nouveau segment de mémoire partagé ou la recherche d'un segment. Le principe est le même qu'avec ```semget()```, mais cette fois-ci pour un segment et pas un semaphore. \
        Le prototype ```int shmget(key_t key, size_t size, int shmflg)```, prend comme argument :
        - arg1: Une clé, qui peut avoir la valeur spéciale ```IPC_PRIVATE``` de type ```key_t``` ou une clé obtenus avec ```ftok()```
        - arg2: La taille du segment.
        - arg3: Des options pour modifier la création que l'on souhaite *(voir le tableau suivant).*
          
          Valeur de option | Explication 
          --- |----------
          Droits | Un nouveau segment de mémoire privé est systématiquement créé. 
          ```IPC_CREAT \| IPC_EXCL \| droits``` | Création d’un nouveau segment de mémoire associé à la clé passée en paramètre. Si le segment existe déjà, une erreur est détectée.
          ```IPC_CREAT \| droits``` | Récupération de l’identification d’un segment de mémoire existant dont la clé est passée en paramètre. Si le segmentn’existe pas, il est créé.
          *Avec droits la valeurs de nos droits, exmeple: 0600. Voir [ici](https://chmodcommand.com/) pour plus d'info.* 
      
        La fonction ```shmget()``` retourne l'id du segment partagé.
      

   2. **La primitive ```shmat()```** :
    
        Permet d'attacher notre segment partagé (créé avec ```shmget()```) à l'espace d'adressage du processus appelant. \
        Le prototype ```void* shmat(int shmid, const void *shmaddr, int shmflg)```, prend comme argument :
        - arg1: ID du segment partagé obtenus dans la valeur de retour de ```shmget()```.
        - arg2: L'adresse où attacher le segment. Si ```NULL``` c'est le processus appelant qui choisis l'addresse, sinon *rtfm* :)
        - arg3: Si l'option ```SHM_RDONLY``` est donnée alors le processus peux uniquement accéder au segment en lecture. 

        La fonction ```shmat()``` retourne avec succès l'addr où est attaché le segment partagé, sinon ```(void*)-1```.



   3. **La primitive ```shmctl()```** :

       La primitive ```shmctl()``` permet d’accéder aux informations contenues dans l’entrée de la table des segments de mémoire partagée et d’en modifier certains attributs. \
       Le prototype ```int shmctl(int shmid, int cmd, struct shmid_ds *buf);```, prend comme argument :
       - arg1: ID du segment partagé obtenus dans la valeur de retour de ```shmget()```.
       - arg2: L'opération qu'on souhaite réaliser *(voir le tableau suivant).*
       - arg3: Structure permettant la modification des informations du segment, applicable avec la commande ```IPC_SET```.
      
         Valeur de ```cmd``` | Explication
         --- |----------
         ```IPC_RMID``` | Suppression du segment de mémoire.
         ```IPC_STAT``` | Récupération des informations dans ```shmid_ds```.
         ```IPC_SET``` | Modification des informations du segment avec ```shmid_ds```.
      
         La fonction ```shmctl()``` retourne avec succès 0, sinon -1.


      
   5. **La primitive ```shmdt()```** :

      C'est la fonction contraire de ```shmat()```, cette primitive permet de détacher notre segment partagé attaché à une certaine adresse donnée en arg2 de ```shmat()```. \
      Le prototype ```int shmdt(const void *shmaddr)```, prend comme argument :
       - arg1: L'adresse où est attaché notre segment.

      La fonction ```shmdt()``` retourne avec succès l'addr où était attaché le segment partagé, sinon ```(void*)-1```.

## Utilisation de la mémoire partagé
   - Exemple :

      ```c
      //TODO
      ```


# Les tubes
## Les fonctions des tubes
   1. **La fonction ```pipe()```** :
      
      Permet de créer un tube. \
      Le prototype ```int pipe(int fdinfo[2])```, prend en argument un tableau de deux entiers. On peut utiliser les deux descripteurs fournis à la suite de l'appel de la fonction :
        - ```df[0]``` pour la sortie du tube, soit la lecture
        - ```df[1]``` pour l'entrée du tube, soit l'écriture
      
      *La syntaxe précédente est valable si c'est un tableau de deux entiers portants le nom ```df``` qui est donnée en argument de ```pipe```, si c'est ```tube``` l'argument alors les descripteurs seront ```tube[0]``` et ```tube[1]```*

      La fonction ```pipe()``` retourne avec succès 0, sinon -1.

## Utilisation des tubes
   - Exemple :

      ```c
      #include <stdio.h>
      #include <stdlib.h>
      #include <unistd.h>
      #include <sys/types.h>
      #include <sys/wait.h>
      
      int main(){
        int tube[2];
        pipe(tube);
     
        pid1=fork();
        if(pid1 == 0){
            close(tube[0]); // fermeture en lecture (sortie du tube)
            close(1); // pas d'écriture sur l'écran
            dup(tube[1]); // la sortie standard devient l'entrée du tube
            close(tube[1]); // descripteur inutile après redirection
            execl("/usr/bin/ls", "ls", "-l", NULL);
            exit(0);
        }
        else if(pid1 == -1){
            printf("pid1, ERROR: -1");
            exit(-1);
        }

        pid2=fork();
        if (pid2 == 0){
            close(tube[1]); // fermeture en écriture (entrée du tube)
            close(0); // pas de lecture sur le clavier
            dup(tube[0]); // l'entrée standard devient la sortie du tube
            close(tube[0]); // descripteur inutile après redirection
            execl("/usr/bin/grep", "grep", "toto", NULL);
        }
        else if(pid2 == -1){
            printf("READ, ERROR: -1");
            exit(-1);
        }
      }
      ```

# Les signaux
## Les fonctions des signaux
   1. **La fonction ```signal()```** :
      
      La fonction ```signal()``` permet de modifier le comportement d'un signal, par exemple si on souhaite qu'à la réception du signal, il faut faire une pirouette au lieu d'une galipette. \
      Le prototype ```void ( *signal (int sig, void(*func)(int)) )(int)```, prend comme argument :
        - arg1: Le type de signal dont il faut modifier le comportement.
        - arg2: Le handler, qui est une fonction où l'on met dedans ce que l'on souhaite que le programme fasse à la réception du signal.


   2. **La fonction ```sigaction()``` et la structure ```struct sigaction```** :

      La fonction ```sigaction()``` remplie le même objectif que la fonction ```signal()```, mais c'est une fonction *POSIX (Portable Operating System Interface)*. \
      La structure ```struct sigaction```, est un des arguments de la fonction ```sigaction()```, elle va permettre de modifier le comportement de notre signal. Par exemple pour le handler si on a créé l'implémentation ```struct sigaction action``` on peut faire ```action.sa_handler = &sigHandler``` pour dire qu'à la réception du signal le programme doit faire la fonction ```sigHandler()```.

      Le prototype ```int sigaction(int sig, const struct sigaction *__restrict__ new, struct sigaction *__restrict__ old);```, prend comme argument :
        - arg1: Le type de signal dont il faut modifier le comportement.
        - arg2: L'implémentation de la structure ```struct sigaction```, qu'on aura créé préalablement avec les modifications souhaitées
        - arg3: on va mettre ```null``` pour simplifier les choses.


   4. **La fonction ```kill()```** :

      Permet d'envoyer un signal. \
      Le prototype ```int kill(int pid, int sig)```, prend en argument un tableau de deux entiers. On peut utiliser les deux descripteurs fournis à la suite de l'appel de la fonction :
       - arg1: Le PID du processus qui doit recevoir le signal.
       - arg2: Le signal à émettre *(voir le tableau suivant).*

         Signal | N°  | Commentaires 
         --- |---| ----------------------------------
         ```SIGHUP```  | 1 | Signal émis lors d’une déconnexion
         ```SIGINT```  | 2 | Il est émis à tout processus associé à un terminal de contrôle quand on appuie sur la touche d’interruption ‘Ctrl-C’.
         ```SIGQUIT``` | 3 | Semblable à SIGINT mais le signal est émis quand on appuie sur la touche d’abandon (normalement ‘Ctl-\’).
         ```SIGILL```  | 4 | Instruction illégale.
         ```SIGFPE```  | 8 | Erreur de calcul flottant.
         ```SIGKILL``` | 9 | C’est la seule manière absolument sûre de détruire un processus, puisque ce signal est toujours fatal.
         ```SIGALRM``` | 14 | Signal émis par alarm(int sec) au bout de sec secondes.
         ```SIGTERM``` | 15 | Terminaison logicielle. Il s’agit du signal standard de terminaison.
         ```SIGUSR1``` | 16 | Premier signal à la disposition de l’utilisateur.
         ```SIGUSR2``` | 17 | Deuxième signal à la disposition de l’utilisateur.
         ```SIGCHLD``` | 18 | Ce signal est envoyé au père à la terminaison d’un processus fils.


   5. **La fonction ```pause()```** :
      
        Permet de mettre le processus en pause et d'attendre un signal avant de continuer.
        

## Utilisation des signaux
   - Exemple :

      ```c
      #include <stdio.h>
      #include <signal.h>
      #include <stdlib.h>
      #include <unistd.h>
      #include <sys/types.h>
      #include <sys/wait.h>
      
      int signalHandler(int sig);
        if (sig == SIGUSER1)printf("I do pirouette because I received the signal %d\n", sig);

      int main(){
        struct sigaction action;

        action.sa_handler = &comHandler;
        if (sigaction(SIGUSR1, &action, NULL) == -1){
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        else {
            printf("Communication handler set\n");
        }
     
        fils = fork();
        if (fils == 0){
            printf("The son wait before send the signal\n")
            sleep(5);
            printf("The son send the signal\n")
            kill(getppid(), SIGUSR1);
            exit(0); 
        }
     
        sleep(1);
        printf("The father wait the signal\n")
        pause();
        printf("end...\n")
      }
      ```
     
        > Communication handler set \
          The son wait before send the signal \
          The father wait the signal \
          The son send the signal \
          I do pirouette because I received the signal 16 \
          end...
          
  
# Le code du TP
```c
/** nbOctets.c **/
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
```

```c
/** partage.h **/

#ifndef DEPOT_ENSTA_C_PARTAGE_H
#define DEPOT_ENSTA_C_PARTAGE_H

/* Structure zone representant une zone partagee entre processus ayant un
lien de parente. Exemple: pere-fils => cle privee d’IPC Sys V */
typedef struct
{
  int id;
  void * debut;
  int taille;
} Zone;

/* fonctions publiques */
extern int creerZonePartagee(int taille, Zone * zp);
extern int supprimerZonePartagee(Zone * zp);

#endif //DEPOT_ENSTA_C_PARTAGE_H
```

```c
/** partage.c **/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "partage.h"

/* ---------------------------------------------------
creerZonePartagee(int taille, Zone *zp)
Action : creer une zone de memoire partagee grace
aux IPC SysV, accessible en lecture/ecriture
par le createur. La taille est en octets.
Remarque : cette zone est heritee par un processus fils
Retourne : -1 en cas d’erreur, 0 sinon
remplit la structure zone passee en argument
--------------------------------------------------- */
int creerZonePartagee(int taille, Zone * zp)
{
  if ((zp->id=shmget(IPC_PRIVATE, taille, 0600|IPC_CREAT))==-1){
    perror("shmget (creerZonePartagee)");
    return -1;
  }
  if ((zp->debut=shmat(zp->id, 0, 0))==(void*)-1){
    perror("shmat (creerZonePartagee)");
    shmctl(zp->id, IPC_RMID, NULL);
    return -1; 
  }
  
  zp->taille=taille;
  return 0;
}

/* ------------------------------------------------------
int supprimerZonePartagee(Zone *zp)
Action : liberer la zone partagee
Retourne : -1 en cas d’erreur, 0 sinon
---------------------------------------------------- */
int supprimerZonePartagee(Zone * zp)
{
  if (shmdt(zp->debut)==-1){ 
      perror("shmdt (supprimerZonePartagee)"); 
      return -1; 
  }
  if (shmctl(zp->id, IPC_RMID, NULL)==-1){ 
      perror("shmctl (supprimerZonePartagee)"); 
      return -1; 
  }
  
  return 0;
}
```



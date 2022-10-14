---
title: "TD1 - Création et terminaison des processus"
date: 10 septembre 2022
---

# Ce qu'il faut retenir du TP1

## Table des matières
- [Ce qu'il faut retenir du TP1](#ce-quil-faut-retenir-du-tp1)
  - [Table des matières](#table-des-matières)
  - [La primitive ```fork()```](#la-primitive-fork)
  - [Caractéristique générale d'un processus](#caractéristique-générale-dun-processus)
  - [La primitive ```exit()``` et ```wait()```](#la-primitive-exit-et-wait)
  - [La primitive ```waitpid()```](#la-primitive-waitpid)
  - [Recouvrement d'un processus (```execl()``` et ```execv()```)](#recouvrement-dun-processus-execl-et-execv)

## La primitive ```fork()```
*cf.* [*exo1*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo1/exo1.c)

Si la primitive ```fork()``` est appelée par un processus (père), alors la commande va créer un second processus (fils) héritant d’une partie des caractéristiques du processus père.

- *fork()* a pour valeur de retour :
  - -1 en cas d’échec
  - 0 pour le processus fils (est retourné quand c’est le fils qui est exécuté)
  - Le PID du processus fils (est retourné quand c’est le père qui est exécuté)
  

- ***Exemple :***

    ```c
    #include <stdio.h>
    #include <unistd.h>
    
    int main(){
        int pid = fork(); 
    
        if (pid == -1)
            printf("Unable to create the process\n");
        else if (pid == 0)
            printf("[%d] -- I'm the son process\n", getpid());
        else
            printf("[%d] -- I'm the father process\n", getpid());
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process

## Caractéristique générale d'un processus
*cf.* [*exo2*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo2/exo2.c) *&* [*exo3*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo3/exo3.c)

- ***PID* du processus actuel et celui du père :**

    ```c
    #include <unistd.h>
    
    pid = getpid();
    ppid = getppid();
    ```

- **Propriétaire réel et effectif :**

  Le propriétaire réel correspond à l’utilisateur qui a lancé le processus. \
  Le propriétaire effectif correspond normalement au propriétaire réel, mais celui-ci peut être modifié si nécessaire. Par exemple,
  pour exécuter la commande ```ping``` ou ```sudo``` il est nécessaire d’avoirs les droits *su* comme ces fichiers appartiens a l’utilisateur
  *root*. Grâce à la commande ```setuid()```, il est possible de positionner l’utilisateur effectif au propriétaire du fichier qui est *root*.

  ```c
  #include <unistd.h>
    
  realOwner = getuid();
  beneficialOwner = geteuid();
  ```

- **Le groupe propriétaire réel et effectif :**

    ```c
    #include <unistd.h>
    
    realOwnerGroup = getgid();
    beneficialOwnerGroup = getegid();
    ```

- **Répertoire de travail :**

    Si le tableau ```processusPath``` n’est pas suffisamment grand, alors la commande va échouer

    ```c
    #include <unistd.h>
    
    char processusPath[100];
    getcwd(processusPath, sizeof(processusPath));
    chdir(processusPath); // Useless in this case, fct for change dir 
    ```

## La primitive ```exit()``` et ```wait()```
*cf.* [*exo4*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo4/exo4.c)

- **La primitive ```exit()``` :**

    Cette fonction met fin au processus qui l’a émis. Dans le cas ou ce processus était père, les processus fils deviennent des zombies (état ```defunct```) quand ils se termine, 
    puisque leurs pères sont morts donc ils n’ont pas pu délivrer leurs derniers messages. \
    En attendant que les processus fils soit terminés et puissent passer à l’état zombie, le processus va à l’orphelinat
    et a pour père le processus ```init``` qui a pour PID 1, l’ancêtre de tous les processus du système excepté le processus 0.


- **La primitive ```wait()``` :**
    
    Cette fonction permet à un processus père d’attendre que les processus fils on finit leurs tâches. Grâce à cette fonction les processus fils ne deviennent pas des zombies, il garde le même père comme il est
    seulement en attente. \
    Il est possible de récupérer le statut du code de retour des processus fils si l’on donne une variable partagée en paramètre de ```wait()```.


- ***Exemple :***

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    
    int main(){
        int pid = fork(); 
        int status = 0;
    
        if (pid == -1){
            printf("Unable to create the process\n");
        }
        else if (pid == 0){
            sleep(5);
            printf("[%d] -- I'm the son process\n", getpid());
            exit(0);
        }
        else {
            printf("[%d] -- I'm the father process\n", getpid());
            wait(&status);
        }
        
        printf("exit code of the son process: %d", status);
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process <br>
      exit code of the son process: 0

## La primitive ```waitpid()```
*cf.* [*exo5*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo5/exo5.c)

La fonction ```waitpid()``` a le même principe de fonctionnement que la commande, ```wait``` mais, au lieu d’attendre que tous les processus fils soient terminés. Le processus père attend uniquement
le processus fils renseigné en paramètre de ```waitpid(pid_t pid, int *status_ptr, int options)``` grâce à son pid.

- La valeur de retour de ```waitpid()``` :
  - -1 en cas d’erreur
  - 0 en cas d’échec
  - < 0 le PID du processus fils


- L’argument ```int *status_ptr```:

  À l’aide d’une variable partagée donnée en paramètre de ```waitpid()``` et de fonctions *(cf. tableau suivant)*, il est possible d’obtenir des informations sur la terminaison d’un processus

  Fonction | Interprétation 
  --- | --- 
  WIFEXITED | Valeur non nulle si le processus s’est terminé normalement. 
  WEXITSTATUS | Fournis le code de retour du processus si celui-ci s’est terminé normalement. 
  WIFSIGNALED | Valeur non nulle si le processus s’est terminé à cause d’un signal.
  WTERMSIG | Fournis le numéro du signal ayant provoqué la terminaison du processus.
  ... | ...


- L’argument ```int options```:

  Il est possible de donner des options en paramètre :

  Option | Interprétation
    --- | --- 
  0 | Aucune option.
  WNOHANG | Le processus appelant n’est pas suspendu.
  ... | ...



- ***Exemple :***

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    
    int main(){
        int pid = fork(); 
        int terminaison = 0;
        int retWaitPid = 0;
    
        if (pid == -1){
            printf("Unable to create the process\n");
        }
        else if (pid == 0){
            sleep(5);
            printf("[%d] -- I'm the son process\n", getpid());
            exit(0);
        }
        else {
            printf("[%d] -- I'm the father process\n", getpid());
            retWaitPid = waitpid(pidFils, &terminaison, 0);
        }
        
        printf("PID of the zombie son process: %d\n", retWaitPid);
        
        if (WIFEXITED(terminaison))
            printf("Return code of the child process: %d\n", WEXITSTATUS(terminaison));
        else if (WIFSIGNALED(terminaison))
            printf("ERROR: Number of the signal that caused the process to end: %d\n", WTERMSIG(terminaison));
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process <br>
      PID of the zombie son process: 3735 <br>
      Return code of the child process: 0

## Recouvrement d'un processus (```execl()``` et ```execv()```)
*cf.* [*exo6*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo6/exo6.c) *&* [*exo7*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo7/exo7.c)

La seule solution pour créer un nouveau processus consiste à utiliser le principe de recouvrement. On va recouvrir un processus qui exécute une tâche A, pour qu’il puisse faire une tache B. \
Ce nouveau processus n’en est pas réellement puisqu’il garde le PID du processus initial. 


- **La primitive ```execl()``` :**
  
  Dois prendre en argument une liste finie, respectant la trame suivante :

  - arg0: Le chemin du programme à exécuter. *(exemple: ```/bin/ls```)*
  - arg1: Le nom de la commande à exécuter. *(exemple: ```ls```)*
  - arg2: Peut-être une option, une chaîne de caractère. *(exemple: ```-l```)*
  - arg3: ...
  - argn: ```NULL``` pour marquer la fin.

  Sa valeur de retour est -1 dans le cas d’échec, sinon les commandes suivants ```execl()``` ne sont pas réalisées
  comme le processus initial a une nouvelle tâche.


- ***Exemple avec ```execl()```:***

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
    
  int main () {
      char buf [80];
  
      printf ("[%d]-- Start of the exec() test\n",getpid());
      sprintf (buf,"-- pid=[%d]", getpid());
      execl ("/bin/echo","echo","Execution","of an exec test",buf, NULL);
      printf ("[%d]-- Failure of the execl() function\n",getpid());
      printf ("[%d]-- End of exec() test\n",getpid());
      exit(0);
  }
  ```

  > [4683]-- Start of the exec() test <br>
  Execution of an exec test -- pid=[4683]



- **La primitive ```execv()``` :**

  Dois prendre en argument un vecteur de pointeurs, respectant la trame suivante :

  - argv[0]: Le chemin du programme à exécuter. *(exemple: ```/bin/ls```)*
  - argv[1]: Le nom de la commande à exécuter. *(exemple: ```ls```)*
  - argv[2]: Peut-être une option, une chaîne de caractère. *(exemple: ```-l```)*
  - argv[3]: ...
  - argv[4]: ```NULL``` pour marquer la fin.

  Sa valeur de retour est -1 dans le cas d’échec, sinon les commandes suivants ```execl()``` ne sont pas réalisées
  comme le processus initial a une nouvelle tâche.



- ***Exemple avec ```execv()```:***

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    
    int main () {
        char buf [80];
        char *argv [5];
        argv [0] = "echo";
        argv [1] = "Execution";
        argv [2] = "of an exec test";
        argv [3] = buf;
        argv [4] = NULL;
    
        printf ("[%d]-- Start of the exec() test\n",getpid());
        sprintf (buf,"-- pid=[%d]", getpid());
        execv ("/bin/echo", argv);
        printf ("[%d]-- Failure of the execv() function\n",getpid());
        printf ("[%d]-- End of exec() test\n",getpid());
        exit(0);
    }
    ```

  > [4712]-- Start of the exec() test <br>
  Execution of an exec test -- pid=[4712]

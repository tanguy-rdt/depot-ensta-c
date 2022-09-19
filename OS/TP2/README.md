---
title: "TP2 - Exclusion mutuelle & Sémaphores"
date: 18 septembre 2022
---

## Table des matières

- [Table des matières](#table-des-matières)
- [Création des sémaphores](#création-des-sémaphores)
- [Utilisation des sémaphores](#utilisation-des-sémaphores)
- [Problème des rendez-vous](#problème-des-rendez-vous)
- [Problème du lecteur/rédacteur](#problème-du-lecteurrédacteur)


## Création des sémaphores 
*cf.* [*sema.c*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP2/exo1/sema.c)

 1) **Création d’une clé à partir d’un fichier avec ```ftok()```** 
    
    La commande ```ftok(const char *path, int id)``` permet de créer une clé à partir de l'identité du chemin d'un fichier donné comme premier paramètre et les 8 premiers bits du second paramètre. 
    Dans le cas où la commande s'est déroulé correctement, ```ftok()``` retourne la clé, sinon *-1*.

    ```c
    #include <sys/ipc.h>
    #include <sys/types.h>
    
    key_t key;
    key = ftok("myKey", 1);
    ```
 
 2) **Création d’un ensemble de sémaphores à partir de la clé avec ```semget()```**

    Grâce à la clé précédemment obtenus avec ```ftok()```, on peut créer un ensemble de sémaphores associé à la clé avec ```semget()```. Le prototype ```semget(key_t key, int nsems, int semflg)```,
    prend en premier argument la clé, puis le nombre de sémaphores *(par exemple, si on en veut 2, alors l’ensemble associé à la clé contiendra 2 sémaphores)*. Le dernier argument prend ```IPC_CREAT```, 
    ce qui permet de créer les sémaphores dans le cas où la clé ne serait pas déjà associée à un ensemble.

    ```c
    #include <sys/sem.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    
    int semid = semget(k, nbSem, IPC_CREAT | 0600); // sans le | 0600 ca ne marche pas jsp pq
    ```

 3) **Initialisation des sémaphores avec ```semctl()```**

    ```semctl(int semid, int semnum, int cmd, ...)``` utilise l’id de l’ensemble des sémaphores créé et obtenu avec la fonction ```semget()```. La suite des arguments correspond ensuite au nombre de
    sémaphores que contient l’ensemble *(donc le nombre de sémaphores à initialiser)*, la commande ```SETALL```  pour initialiser tous les sémaphores avec les valeurs données en 4e argument. \
    Le tableau qui contient les valeurs doit être préalablement redimensionné par rapport au nombre de sémaphores à initialiser, puis il faut lui donner les valeurs d’initialisation.
    La ```values[0]``` correspond à la valeur d’initialisation du premier sémaphore.

    ```c
    #include <sys/sem.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    
    unsigned short *values = (unsigned short*)malloc(sizeof(unsigned short)*nbSem);
    for (s=0; s<nbSem; s++) values[s]=1;

    semctl(semid, nbSem, SETALL, values);
    free(values);
    ```

## Utilisation des sémaphores

Pour pouvoir utiliser les sémaphores, il ne reste plus qu’à créer les fonctions ```P()``` et ```V()``` qui peuvent être dans des cas généraux :

```c
int P(int semid, int noSem)
{
    struct sembuf Ops[1];

    Ops[0].sem_num = noSem; // Le numéro du sémaphore qui doit effectuer l'opération
    Ops[0].sem_op = -1; // P donc on décrémente
    Ops[0].sem_flg = 0;
    
    // arg1: l'id de l'ensemble des sémaphores, arg2: opération à effectuer, arg3: nombre d'opération à effectuer
    ret = semop(semid, Ops, 1);
    return ret;
}
```

```c
int V(int semid, int noSem)
{
    struct sembuf Ops[1];
    
    Ops[0].sem_num = noSem; // Le numéro du sémaphore qui doit effectuer l'opération
    Ops[0].sem_op = +1; // V donc on incrémente
    Ops[0].sem_flg = 0;

    // arg1: l'id de l'ensemble des sémaphores, arg2: opération à effectuer, arg3: nombre d'opération à effectuer
    ret = semop(semid, Ops, 1);
    return ret;
}
```

C’est étapes nous permettent maintenant de résoudre des problèmes comme ceux des [rendez-vous](#problème-des-rendez-vous).


## Problème des rendez-vous
*cf.* [*exo1*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP2/exo1)
```c
/** premier processus (RDV1) **/
int main (void){ 
    key_t k;
    k = ftok("myKey", PRJVAL); // on récupére la clé utilsée pour créer les sémaphores avec sema.c
    int semid = semget(k, 0, 0); // arg2 et arg3 à 0 comme ce n'est pas une création
    
    sleep(1);
    V(semid, 0);
    P(semid, 1);
    printf("RDV1");

    return 0;
}
```

```c
/** second processus (RDV2) **/
int main (void){
    key_t k;
    k = ftok("myKey", PRJVAL); // on récupére la clé utilsée pour créer les sémaphores avec sema.c
    int semid = semget(k, 0, 0); // arg2 et arg3 à 0 comme ce n'est pas une création
    
    sleep(5);
    V(semid, 1);
    P(semid, 0);
    printf("RDV2");

    semctl(semid, 2, IPC_RMID); // suppression des sémaphores de l'ensemble au id semid
    return 0;
}
```

## Problème du lecteur/rédacteur
*cf.* [*exo2*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP2/exo2)





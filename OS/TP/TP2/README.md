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
    
    int semid = semget(k, nbSem, IPC_CREAT | 0600); // 0600 pour autoriser lecture et écriture
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
```c
/** premier processus (redacteur) **/

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

int main (void)
{
    const char *filename = "myFile.txt";
    int stateMutexR;
    
    key_t k;
    k = ftok("myKey", PRJVAL); // On récupère la même clé que celle utilisée pour la création des sem
    
    int semid = semget(k, 0, 0); // On ouvre l'esemble des sem avec la clé et on recup l'id
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);
    
    P(semid, MUTEX_R); // On prend le rédacteur pour bloquer les lecteurs
    sleep(10); // sleep bidon
    ecrire(filename); // on écris dans le fichier
    V(semid, MUTEX_R); // On libére le R pour permettre au L de lire le fichier
    
    //semctl(semid, 2, IPC_RMID); // suppression des sem
    return 0;
}
```

```c
/** second processus (lecteur) **/

/** fonction pour mettre a jour le nb de lecteur **/
int update_nb_lecteur(int val){
    int state;
    
    FILE *fp;
    fp = fopen("nombre", "w"); // ouverture/création du fichier, le contenu est effacé si le fichier existe
    state = fprintf(fp, "%d", val); // écriture du nombre de lecteur actif dans le fichier
    fclose(fp); // fermeture du fichier
    
    if (state < 0){
        printf("ERROR: Nothing was written\n");
    }
    return state;
}

/** fonction pour obtenir le nb de lecteur actif **/
int get_nb_lecteur(){
    int nb;
    int state;
    
    FILE *fp;
    fp = fopen("nombre", "r"); // ouverture du fichier pour lire le conteu, le fichier doit existé
    state = fscanf(fp, "%d", &nb); // on récupére le nombre de lecteur et on le stock dans la var nb
    fclose(fp); // fermeture du fichier
    
    if (state == 0){
        printf("ERROR: Nothing to read\n");
    }
    return nb; // on retourne le nombre de fichier
}

/** fonction pour lire le fichier **/
int lire(const char *filename){
    char str1[6], str2[6], str3[2], str4[4], str5[4], str6[9]; // var pour stocker le contenu
    int date, year; // var pour stocker le contenu
    int state;
    
    FILE *fp;
    fp = fopen(filename, "r"); // ouverture du fichier pour lire le conteu, le fichier doit existé
    state = fscanf(fp, "%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, &date, str6, &year); // on lit le contenu
    printf("%s %s %s %s %s %d %s %d", str1, str2, str3, str4, str5, date, str6, year); // on affiche le contenu
    fclose(fp); // on ferme le fichier
    
    if (state == 0){
        printf("ERROR: Nothing to read\n");
    }
    return state;
}

int main (void)
    {
    const char *filename = "myFile.txt";
    int nb_lecteur;
    
    key_t k;
    k = ftok("myKey", PRJVAL); // On récupère la même clé que celle utilisée pour la création des sem
    
    int semid = semget(k, 0, 0); // On ouvre l'esemble des sem avec la clé et on recup l'id
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);
    
    P(semid, MUTEX_L); // On prend le lecteur pour bloquer la modification du nombre de lecteur
    nb_lecteur = get_nb_lecteur(); // on récupère le nb de lecteur
    update_nb_lecteur(nb_lecteur+1); // on incrémente le nombre de lecteur
    if(get_nb_lecteur() == 1){
        printf("%d", P(semid, MUTEX_R)); // si c'est le premier lecteur alors on prend le redacteur pour eviter une modif
    }
    V(semid, MUTEX_L); // on libére le lecteur
    
    sleep(5); // sleep bidon
    lire(filename); // on lit le fichier
    
    P(semid, MUTEX_L); // On prend le lecteur pour bloquer la modification du nombre de lecteur
    nb_lecteur = get_nb_lecteur(); // on récupère le nb de lecteur
    update_nb_lecteur(nb_lecteur-1); // on décrémente le nombre de lecteur
    if(get_nb_lecteur() == 0){
        V(semid, MUTEX_R); // si c'est le dernier lecteur on libére le redacteur pour une modif potentiel
    }
    V(semid, MUTEX_L);  // on libére le lecteur
    
    //semctl(semid, 2, IPC_RMID); // supp des sem
    return 0;
}
```




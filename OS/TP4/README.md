---
title: "TP4 - Threads POSIX"
date: 04 octobre 2022
---

# Table des matières

- [Table des matières](#table-des-matières)
- [Fonctions](#fonctions)
- [Exemple](#exemple)
- [Code du TP](#code-du-tp)


# Fonctions
## Création d'un thread 

1. ```pthread_create()``` 

   Permet la création d'un thread. \
   Le prototype ```int pthread_create(pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg)```, prends comme argument :
  
     - *arg1*: Une variable de type ```pthread_t myTh```, c'est en quelque sorte un lien avec le thread, l'identifiant du thread créé est dans cette variable.
     - *arg2*: Objet attribut comme une politique d'ordonnancement, par exemple. ```NULL```, si rien à donner
     - *arg3*: Une fonction que le thread doit réaliser
     - *arg4*: L'argument pris par la fonction que le thread doit réaliser, donné à l'arg3

   La fonction retourne 0 pour un succès sinon -1


## Les mutex

 1. ```pthread_mutex_init()```

    Permet d'initialiser un mutex. \
    Le prototype ```int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr)```, prends comme argument :

     - *arg1*: Une variable de type ```pthread_mutex_t* myMutex```, c'est en quelque sorte un lien avec le mutex, l'identifiant du mutex créé est dans cette variable.
     - *arg2*: Un attribut, dans le cas où *arg2* est ```NULL``` alors c'est l'attribut ```NONRECURSIVE``` qui est utilisé.

    La fonction retourne 0 pour un succès sinon -1


 2. ```pthread_mutex_lock()```

    Permet de verrouiller un mutex. \
    Le prototype ```int pthread_mutex_lock(pthread_mutex_t *mutex)```, prends comme argument :

    - *arg1*: Le mutex à verrouiller, précédemment initialiser avant avec ```pthread_mutex_init()```

    La fonction retourne 0 pour un succès sinon -1


 3. ```pthread_mutex_unlock()```

    Permet de libérer un mutex. \
    Le prototype ```int pthread_mutex_unlock(pthread_mutex_t *mutex)```, prends comme argument :

    - *arg1*: Le mutex à déverrouiller, précédemment verrouiller avant avec ```pthread_mutex_lock()```

    La fonction retourne 0 pour un succès sinon -1


 5. ```pthread_mutex_destroy()```

    Permet de détruire un mutex. \
    Le prototype ```int pthread_mutex_destroy(pthread_mutex_t *mutex);```, prends comme argument :

    - *arg1*: Le mutex à détruire.

    La fonction retourne 0 pour un succès sinon -1


## Les conditions

1. ```pthread_cond_init()```

    Permet d'initialiser une condition. \
    Le prototype ```int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr)```, prends comme argument :

    - *arg1*: Une variable de type ```pthread_cond_t* myCond```, c'est la variable à utiliser à chaque fois que l'on veut intéragir avec la condition.
    - *arg2*: Objet attribut. ```NULL```, si rien à donner

   La fonction retourne 0 pour un succès sinon -1


2. ```pthread_cond_wait()```

    Permet d'attendre qu'une condition soit effectuée. \
    Le prototype ```int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)```, prends comme argument :

   - *arg1*: La variable condition, utilisée lors de l'initialisation avec  ```pthread_cond_init()```.
   - *arg2*: Un mutex pour l'associer à la condition.

   La fonction retourne 0 pour un succès sinon -1

   À l’appel de cette fonction, il est nécessaire de verrouillé le mutex associé *(voir ```pthread_mutex_lock()```)* avant et de le libérer après *(voir ```pthread_mutex_unlock()```)*.


3. ```pthread_cond_broadcast()```

    Permet d'envoyer un signal à toutes les conditions qui attendent, pour dire que la condition est remplie. \
    Le prototype ```int pthread_cond_broadcast(pthread_cond_t *cond)```, prends comme argument :

    - *arg1*: La condition qui a été remplie, donc il faut envoyer un signal au thread attendant cette condition.

   La fonction retourne 0 pour un succès sinon -1

   À l’appel de cette fonction, il est nécessaire de verrouillé le mutex associé *(voir ```pthread_mutex_lock()```)* avant et de le libérer après *(voir ```pthread_mutex_unlock()```)*.


5. ```pthread_cond_destroy()```

    Permet de détruire une condition. \
    Le prototype ```int pthread_cond_destroy(pthread_cond_t *cond)```, prend comme argument :

    - *arg1*: La condition à détruire.

   La fonction retourne 0 pour un succès sinon -1



## Attendre la fin d'un thread

```pthread_join()```, permet d'attendre la fin de l'exécution d'un thread. \
Le prototype ```int pthread_join(pthread_t thread, void **status)```, prends comme argument :

 - *arg1*: Le thread à attendre.
 - *arg2*: Contient la valeur de retour du thread renvoyer par ```pthread_exit()```, ```return```, ou -1 dans le cas d'une annulation.

La fonction retourne 0 pour un succès sinon -1


# Exemple

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int halfThread1 = 0;

int *thread2(void *arg)
{
    // wait the half of the thread1 (5sec)
    pthread_mutex_lock(&mutex);
    while(!halfThread1){ pthread_cond_wait(&cond, &mutex);}
    pthread_mutex_unlock(&mutex);

    // start the prog of the thread2
    printf("start thread2 \n");
    sleep(10);
    printf("end thread2 \n");

    return 2;
}

int *thread1(void *arg)
{
    // start the prog of the thread 1
    printf("start thread1 \n");
    sleep(5);

    // half of the thread1 --> start thread2
    printf("half of the thread1 \n");
    halfThread1 = 1;
    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    sleep(5);
    printf("end thread1 \n");

    return 1;
}


int main (void)
{
    printf("------------------- Start -------------------\n");

    pthread_t thr1, thr2;
    int* retThread1;
    int* retThread2;

    // init the mutex and cond
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // create the thread
    if ((pthread_create(&thr1, NULL, thread1, NULL)) || (pthread_create(&thr2, NULL, thread2, NULL))!= 0) {
        fprintf(stderr, "Error during pthread_create()\n");
        exit(EXIT_FAILURE);
    }

    // wait the end of the thread, return the exit value in arg2
    pthread_join(thr1, &retThread1);
    pthread_join(thr2, &retThread2);

    // destroy the mutex and cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("return value of the thread1: %d\n", retThread1);
    printf("return value of the thread2: %d\n", retThread2);

    printf("-------------------- End --------------------");

    return 0;
}
```

> ------------------- Start ------------------- \
  start thread1 \
  half of the thread1 \
  start thread2 \
  end thread1 \
  end thread2 \
  return value of the thread1: 1 \
  return value of the thread2: 2 \
  -------------------- End --------------------


# Code du TP
*cf.* [*TP4*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP4)

```c
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*********** Data Type ***********/
typedef enum
{
STATE_WAIT,
STATE_MULT,
STATE_ADD,
STATE_PRINT
} State;

typedef struct
{
State state;
int * pendingMult;
pthread_cond_t cond;
pthread_mutex_t mutex;
size_t nbIterations;
size_t size;
double * v1;
double * v2;
double * v3;
double result;
} Product;

/*********** Data structure ***********/
Product prod;

/*********** Function ***********/
void initPendingMult(Product* prod){
    size_t i;
    for(i=0;i<prod->size;i++){
        prod->pendingMult[i]=1;
    }
}

int nbPendingMult(Product* prod){
    size_t i;
    int nb=0;
    for(i=0;i<prod->size;i++){
        nb+=prod->pendingMult[i];
    }
    return(nb);
}

void wasteTime(unsigned long ms){
    unsigned long t,t0;
    struct timeval tv;
    gettimeofday(&tv,(struct timezone *)0);
    t0=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
    do{
        gettimeofday(&tv,(struct timezone *)0);
        t=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
    } while(t-t0<ms);
}

void *mult(void * data){
    size_t index;
    size_t iter;
    
    //TODO: Recuperation de l'index, c'est a dire index = ...
    index = (size_t)data;
    fprintf(stderr,"Begin mult(%d)\n",index);
    
    for(iter=0;iter<prod.nbIterations;iter++){
        //TODO: Attendre l'autorisation de multiplication POUR UNE NOUVELLE ITERATION...
        pthread_mutex_lock(&prod.mutex);
        while((prod.state != STATE_MULT) || (nbPendingMult(&prod) != prod.size)){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);
        
        fprintf(stderr,"--> mult(%d)\n",index);
        
        
        //TODO: Effectuer la multiplication a l'index du thread courant...
        prod.v3[index] = prod.v1[index] * prod.v2[index];
        
        wasteTime(200+(rand()%200));
        
        fprintf(stderr,"<-- mult(%d) : %.3g*%.3g=%.3g\n", index,prod.v1[index],prod.v2[index],prod.v3[index]);
        
        
        //TODO: Marquer la fin de la multiplication en cours...
        pthread_mutex_lock(&prod.mutex);
        prod.pendingMult[index] = 0;
        pthread_mutex_unlock(&prod.mutex);
        
        //TODO: Si c'est la derniere...
        if (nbPendingMult(&prod) == 0){
            //TODO: Autoriser le demarrage de l'addition...
            pthread_mutex_lock(&prod.mutex);
            prod.state = STATE_ADD;
            pthread_cond_broadcast(&prod.cond);
            pthread_mutex_unlock(&prod.mutex);
        }
    }
    fprintf(stderr,"Quit mult(%d)\n",index);
    
    return(data);
}

void * add(void * data){
    size_t iter;
    fprintf(stderr,"Begin add()\n");
    
    for(iter=0;iter<prod.nbIterations;iter++){
        size_t index;
        
        //TODO: Attendre l'autorisation d'addition...
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_ADD){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);
        
        fprintf(stderr,"--> add\n");
        
        prod.result=0.0;
        for(index=0;index<prod.size;index++){
            //TODO: Effectuer l'addition...
            prod.result += prod.v3[index];
        }
        
        wasteTime(100+(rand()%100));
        
        fprintf(stderr,"<-- add\n");
        
        //TODO: Autoriser le demarrage de l'affichage...
        pthread_mutex_lock(&prod.mutex);
        prod.state = STATE_PRINT;
        pthread_cond_broadcast(&prod.cond);
        pthread_mutex_unlock(&prod.mutex);
    }
    fprintf(stderr,"Quit add()\n");
    
    return(data);
}

/*********** Main ***********/
int main(int argc,char ** argv)
{
    size_t i, iter;
    pthread_t *multTh;
    size_t    *multData;
    pthread_t  addTh;
    void      *threadReturnValue;
    
    (void)addTh; (void)threadReturnValue;
    
    /* Lire le nombre d'iterations et la taille des vecteurs */
    if((argc<=2)||
    (sscanf(argv[1],"%u",&prod.nbIterations)!=1)||
    (sscanf(argv[2],"%u",&prod.size)!=1)||
    ((int)prod.nbIterations<=0)||((int)prod.size<=0))
    {
        fprintf(stderr,"usage: %s nbIterations vectorSize\n",argv[0]);
        return(EXIT_FAILURE);
    }
    
    /* Initialisations (Product, tableaux, generateur aleatoire,etc) */
    prod.state=STATE_WAIT;
    prod.pendingMult=(int *)malloc(prod.size*sizeof(int));
    
    //TODO: initialiser prod.mutex...
    pthread_mutex_init(&prod.mutex, NULL);
    
    //TODO: initialiser prod.cond...
    pthread_cond_init(&prod.cond, NULL);
    
    /* Allocation dynamique des 3 vecteurs v1, v2, v3 */
    prod.v1=(double *)malloc(prod.size*sizeof(double));
    prod.v2=(double *)malloc(prod.size*sizeof(double));
    prod.v3=(double *)malloc(prod.size*sizeof(double));
    
    /* Allocation dynamique du tableau pour les threads multiplieurs */
    multTh=(pthread_t *)malloc(prod.size*sizeof(pthread_t));
    
    /* Allocation dynamique du tableau des MulData */
    multData=(size_t *)malloc(prod.size*sizeof(size_t));
    
    /* Initialisation du tableau des MulData */
    for(i=0;i<prod.size;i++){
        multData[i]=i;
    }
    
    //TODO: Creer les threads de multiplication...
    for(i=0; i<prod.size; i++){
        if (pthread_create(multTh+i, NULL, mult, (size_t*)multData[i])  != 0) {
            fprintf(stderr, "Error during pthread_create()\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //TODO: Creer le thread d'addition...
    pthread_create(&addTh, NULL, add, NULL);
    
    srand(time((time_t *)0));   // Init du generateur de nombres aleatoires
    
    for(iter=0;iter<prod.nbIterations;iter++){
        size_t j;
        
        // Initialiser aleatoirement les deux vecteurs
        for(j=0;j<prod.size;j++){
            prod.v1[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));
            prod.v2[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));
        }
        
        //TODO: Autoriser le demarrage des multiplications pour une nouvelle iteration...
        initPendingMult(&prod);
        pthread_mutex_lock(&prod.mutex);
        prod.state = STATE_MULT;
        pthread_cond_broadcast(&prod.cond);
        pthread_mutex_unlock(&prod.mutex);
        
        //TODO: Attendre l'autorisation d'affichage...
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_PRINT){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);
        
        //TODO: Afficher le resultat de l'iteration courante...
        fprintf(stderr,"ITERATION %d, RESULT=%.4g\n\n", iter, prod.result);
    }
    
    //TODO: Attendre la fin des threads de multiplication...
    for(i=0; i<prod.size; i++){
        pthread_join(*(multTh+i), NULL);
    }
    
    //TODO: Attendre la fin du thread d'addition...
    pthread_join(addTh, NULL);
    
    //TODO: detruire prod.cond ...
    pthread_cond_destroy(&prod.cond);
    
    //TODO: detruire prod.mutex...
    pthread_mutex_destroy(&prod.mutex);
    
    /* Detruire avec free ce qui a ete initialise avec malloc */
    free(prod.pendingMult);
    free(prod.v1);
    free(prod.v2);
    free(prod.v3);
    free(multTh);
    free(multData);
    return(EXIT_SUCCESS);
}
```

> Begin mult(0)\
  Begin mult(1)\
  Begin mult(2)\
  Begin mult(3)\
  Begin add()\
  --> mult(0)\
  --> mult(1)\
  --> mult(2)\
  --> mult(3)\
  <-- mult(2) : 2.65*4.85=12.8\
  <-- mult(1) : -3.39*-1.31=4.45\
  <-- mult(0) : 2.62*3.94=10.3\
  <-- mult(3) : -0.365*0.701=-0.256\
  --> add\
  <-- add\
  ITERATION 0, RESULT=27.36\
   \
  --> mult(3)\
  --> mult(1)\
  --> mult(2)\
  --> mult(0)\
  <-- mult(0) : -2.64*1.04=-2.74\
  <-- mult(1) : 4.55*1.06=4.84\
  <-- mult(2) : 0.591*-1.8=-1.06\
  <-- mult(3) : -4.83*3.86=-18.6\
  --> add\
  <-- add\
  ITERATION 1, RESULT=-17.6\
   \
  --> mult(2)\
  --> mult(1)\
  --> mult(3)\
  --> mult(0)\
  <-- mult(2) : 3.88*4.95=19.2\
  Quit mult(2)\
  <-- mult(0) : 3.95*-0.27=-1.07\
  Quit mult(0)\
  <-- mult(1) : 0.963*2.44=2.35\
  Quit mult(1)\
  <-- mult(3) : 3.33*3.97=13.2\
  Quit mult(3)\
  --> add\
  <-- add\
  Quit add()\
  ITERATION 2, RESULT=33.73





//
// Created by Tanguy Roudaut on 29/09/2022.
//
#define _GNU_SOURCE

#include <sched.h>
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
    STATE_PRINT,
    STATE_START
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
int getAffinityProcess(pid_t pid, cpu_set_t cpuSet){
    char *state;

    int nCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU_ZERO(&cpuSet);
    if (sched_getaffinity(pid, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }

    printf("CPU affinity:\n");
    for (int i = 0; i < nCPU; i++) {
        if (CPU_ISSET(i, &cpuSet)) state = "true";
        else state = "false";
        printf("\t CPU %d: %s\n", i, state);
    }

    return 0;
}

int setAffinityProcess(pid_t pid, int numCPU, cpu_set_t cpuSet){
    printf("Set affinity for the CPU %d\n", numCPU);

    CPU_ZERO(&cpuSet);
    CPU_SET(numCPU, &cpuSet);
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_setaffinity");
        exit(-1);
    }

    return 0;
}

int getAffinityThread(pthread_t th, cpu_set_t cpuSet){
    char *state;

    int nCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU_ZERO(&cpuSet);
    if (pthread_getaffinity_np(th, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }

    printf("CPU affinity:\n");
    for (int i = 0; i < nCPU; i++) {
        if (CPU_ISSET(i, &cpuSet)) state = "true";
        else state = "false";
        printf("\t CPU %d: %s\n", i, state);
    }

    return 0;
}

int setAffinityThread(pthread_t th, int numCPU, cpu_set_t cpuSet){
    CPU_ZERO(&cpuSet);
    if (pthread_getaffinity_np(th, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }

    if (CPU_ISSET(numCPU, &cpuSet)) {
        printf("CPU %d is already used for this thread\n", numCPU);
        return 1;
    }
    else {
        printf("Set affinity for the CPU %d\n", numCPU);
        CPU_ZERO(&cpuSet);
        CPU_SET(numCPU, &cpuSet);
        if (pthread_setaffinity_np(th, sizeof(cpu_set_t), &cpuSet) == -1) {
            perror("sched_setaffinity");
            exit(-1);
        }
    }


    return 0;
}

void reportAffinityCPU(pthread_t addTh, pthread_t* multTh, cpu_set_t cpuSet){
    int nCPU = sysconf(_SC_NPROCESSORS_ONLN);
    int CPU;

    printf("---------------------------------\n");
    printf("|  Process/Thread  | CPU number |\n");
    printf("=================================\n");
    CPU_ZERO(&cpuSet);
    if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }
    for (int numCPU = 0; numCPU < nCPU; numCPU++) {
        if (CPU_ISSET(numCPU, &cpuSet)) {
            CPU = numCPU;
            break;
        }
    }
    printf("| Calling process  |      %d     |\n", CPU);
    for(int numThread=0; numThread<prod.size; numThread++){
        CPU_ZERO(&cpuSet);
        if (pthread_getaffinity_np(*(multTh+numThread), sizeof(cpu_set_t), &cpuSet) == -1) {
            perror("sched_getaffinity");
            exit(-1);
        }
        for (int numCPU = 0; numCPU < nCPU; numCPU++) {
            if (CPU_ISSET(numCPU, &cpuSet)) {
                CPU = numCPU;
                break;
            }
        }

        printf("|  Thread mult %d   |      %d     |\n", numThread, CPU);
    }
    CPU_ZERO(&cpuSet);
    if (pthread_getaffinity_np(addTh, sizeof(cpu_set_t), &cpuSet) == -1) {
        perror("sched_getaffinity");
        exit(-1);
    }
    for (int numCPU = 0; numCPU < nCPU; numCPU++) {
        if (CPU_ISSET(numCPU, &cpuSet)) {
            CPU = numCPU;
            break;
        }
    }
    printf("|   Thread add     |      %d     |\n", CPU);
    printf("---------------------------------\n\n");
}

void initPendingMult(Product* prod)
{
    size_t i;
    for(i=0;i<prod->size;i++)
    {
        prod->pendingMult[i]=1;
    }
}

int nbPendingMult(Product* prod)
{
    size_t i;
    int nb=0;
    for(i=0;i<prod->size;i++)
    {
        nb+=prod->pendingMult[i];
    }
    return(nb);
}

void wasteTime(unsigned long ms)
{
    unsigned long t,t0;
    struct timeval tv;
    gettimeofday(&tv,(struct timezone *)0);
    t0=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
    do
    {
        gettimeofday(&tv,(struct timezone *)0);
        t=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
    } while(t-t0<ms);
}

void *mult(void * data)
{
    //Waiting the end of the CPU affinity
    pthread_mutex_lock(&prod.mutex);
    while((prod.state != STATE_START) && (prod.state != STATE_MULT)){pthread_cond_wait(&prod.cond, &prod.mutex);}
    pthread_mutex_unlock(&prod.mutex);

    size_t index;
    size_t iter;

    //DONE: Recuperation de l'index, c'est a dire index = ...
    index = (size_t)data;
    fprintf(stderr,"Begin mult(%d)\n",index);

    for(iter=0;iter<prod.nbIterations;iter++)
    {
        //DONE: Attendre l'autorisation de multiplication POUR UNE NOUVELLE ITERATION...
        pthread_mutex_lock(&prod.mutex);
        while((prod.state != STATE_MULT) || (nbPendingMult(&prod) != prod.size)){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);

        fprintf(stderr,"--> mult(%d)\n",index); // La multiplication peut commencer

        //DONE: Effectuer la multiplication a l'index du thread courant...
        prod.v3[index] = prod.v1[index] * prod.v2[index];

        wasteTime(200+(rand()%200)); // Perte du temps avec wasteTime()

        fprintf(stderr,"<-- mult(%d) : %.3g*%.3g=%.3g\n", index,prod.v1[index],prod.v2[index],prod.v3[index]);//Affichage du calcul sur l'index


        //DONE: Marquer la fin de la multiplication en cours...
        pthread_mutex_lock(&prod.mutex);
        prod.pendingMult[index] = 0;
        pthread_mutex_unlock(&prod.mutex);

        //DONE: Si c'est la derniere...
        if (nbPendingMult(&prod) == 0){
            //DONE: Autoriser le demarrage de l'addition...
            pthread_mutex_lock(&prod.mutex);
            prod.state = STATE_ADD;
            pthread_cond_broadcast(&prod.cond);
            pthread_mutex_unlock(&prod.mutex);
        }
    }
    fprintf(stderr,"Quit mult(%d)\n",index);
    return(data);
}

void * add(void * data)
{
    //Waiting the end of the CPU affinity
    pthread_mutex_lock(&prod.mutex);
    while((prod.state != STATE_START) && (prod.state != STATE_MULT)){pthread_cond_wait(&prod.cond, &prod.mutex);}
    pthread_mutex_unlock(&prod.mutex);

    size_t iter;
    fprintf(stderr,"Begin add()\n");

    /* Tant que toutes les iterations */
    for(iter=0;iter<prod.nbIterations;iter++)  // n'ont pas eu lieu
    {
        size_t index;

        //DONE: Attendre l'autorisation d'addition...
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_ADD){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);

        fprintf(stderr,"--> add\n"); // L'addition peut commencer

        prod.result=0.0;
        for(index=0;index<prod.size;index++)
        {
            //DONE: Effectuer l'addition...
            prod.result += prod.v3[index];
        }

        wasteTime(100+(rand()%100)); // Perdre du temps avec wasteTime()

        fprintf(stderr,"<-- add\n");

        //DONE: Autoriser le demarrage de l'affichage...
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

/* A cause de warnings lorsque le code n'est pas encore la...*/
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

    cpu_set_t cpuSet;
    int stateAffinityCPU;

/* Initialisations (Product, tableaux, generateur aleatoire,etc) */
    prod.state=STATE_WAIT;
    prod.pendingMult=(int *)malloc(prod.size*sizeof(int));

//DONE: initialiser prod.mutex...
    pthread_mutex_init(&prod.mutex, NULL);

//DONE: initialiser prod.cond...
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
    for(i=0;i<prod.size;i++)
    {
        multData[i]=i;
    }

//DONE: Gestion du CPU pour le processus appelant...
    printf("---------------- Set up CPU Affinity -----------------\n\n");
    printf("Set up CPU affinity for the calling process:\n", i);
    getAffinityProcess(getpid(), cpuSet);
    setAffinityProcess(getpid(), 0, cpuSet);
    getAffinityProcess(getpid(), cpuSet);
    printf("\n");

//DONE: Creer les threads de multiplication...
    int nCPU = sysconf(_SC_NPROCESSORS_ONLN);
    for(i=0; i<prod.size; i++){
        if (pthread_create(multTh+i, NULL, mult, (size_t*)multData[i])  != 0) {
            fprintf(stderr, "Error during pthread_create()\n");
            exit(EXIT_FAILURE);
        }

//DONE: Gestion du CPU pour le thread de multiplication...
        printf("Set up CPU affinity for the multiplication thread, index %d:\n", i);
        getAffinityThread(*(multTh+i), cpuSet);
        stateAffinityCPU = setAffinityThread(*(multTh+i), i%nCPU, cpuSet);
        if(!stateAffinityCPU) getAffinityThread(*(multTh+i), cpuSet);
        printf("\n");

    }

//DONE: Creer le thread d'addition...
    if (pthread_create(&addTh, NULL, add, NULL) != 0){
        fprintf(stderr, "Error during pthread_create()\n");
        exit(EXIT_FAILURE);
    }

//DONE: Gestion du CPU pour le thread d'addition...
    printf("Set up CPU affinity for the addition thread:\n");
    getAffinityThread(addTh, cpuSet);
    stateAffinityCPU = setAffinityThread(addTh, 1, cpuSet);
    if(!stateAffinityCPU) getAffinityThread(addTh, cpuSet);
    printf("\n");
    printf("----------------- Report CPU Affinity ----------------\n\n");

    reportAffinityCPU(addTh, multTh, cpuSet);

    printf("------------------ End CPU Affinity ------------------\n\n");

    printf("-------------- Start the scalar product --------------\n\n");

    pthread_mutex_lock(&prod.mutex);
    prod.state = STATE_START;
    pthread_cond_broadcast(&prod.cond);
    pthread_mutex_unlock(&prod.mutex);

    srand(time((time_t *)0));   // Init du generateur de nombres aleatoires

// Pour chacune des iterations a realiser
    for(iter=0;iter<prod.nbIterations;iter++)
    {
        size_t j;

        // Initialiser aleatoirement les deux vecteurs
        for(j=0;j<prod.size;j++)
        {
            prod.v1[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));
            prod.v2[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));
        }

        //DONE: Autoriser le demarrage des multiplications pour une nouvelle iteration...
        initPendingMult(&prod);
        pthread_mutex_lock(&prod.mutex);
        prod.state = STATE_MULT;
        pthread_cond_broadcast(&prod.cond);
        pthread_mutex_unlock(&prod.mutex);

        //DONE: Attendre l'autorisation d'affichage...
        pthread_mutex_lock(&prod.mutex);
        while(prod.state != STATE_PRINT){pthread_cond_wait(&prod.cond, &prod.mutex);}
        pthread_mutex_unlock(&prod.mutex);

        //DONE: Afficher le resultat de l'iteration courante...
        fprintf(stderr,"ITERATION %d, RESULT=%.4g\n\n", iter, prod.result);
    }

//DONE: Attendre la fin des threads de multiplication...
    for(i=0; i<prod.size; i++){
        pthread_join(*(multTh+i), NULL);
    }

//DONE: Attendre la fin du thread d'addition...
    pthread_join(addTh, NULL);

//DONE: detruire prod.cond ...
    pthread_cond_destroy(&prod.cond);

//DONE: detruire prod.mutex...
    pthread_mutex_destroy(&prod.mutex);

/* Detruire avec free ce qui a ete initialise avec malloc */
    free(prod.pendingMult);
    free(prod.v1);
    free(prod.v2);
    free(prod.v3);
    free(multTh);
    free(multData);
    printf("-------------- End the scalar product --------------\n\n");

    return(EXIT_SUCCESS);
}

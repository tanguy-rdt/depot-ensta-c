//
// Created by Tanguy Roudaut on 29/09/2022.
//

// ------------------------------------------------------------------
// exemple-pthread-create-1.c
// Fichier d'exemple du livre "Developpement Systeme sous Linux"
// (C) 2000-2019 - Christophe BLAESS <christophe@blaess.fr>
// https://www.blaess.fr/christophe/
// ------------------------------------------------------------------

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t verrou;

void *thread_function(void *arg)
{
    while (1) {
        fprintf(stderr, "New thread %d\n", (int)arg);
        sleep(1);
    }
}


int main (void)
{
    pthread_t thr;
    size_t    multData[5];
    size_t    i;


    for(i=1; i<=5; i++){
        multData[i] = i;
    }


    for(i=1; i<=5; i++){
        if (pthread_create(&thr, NULL, thread_function, multData[i]) != 0) {
            fprintf(stderr, "Error during pthread_create()\n");
            exit(EXIT_FAILURE);
        }

        pthread_join(&thr, NULL);
    }


    while (1) {
        fprintf(stderr, "Main thread\n");
        sleep(2);
    }
    pthread_mutex_destroy(&verrou);
}
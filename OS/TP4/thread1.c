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
        sleep(2);
    }
}


int main (void)
{
    pthread_t thr;

    pthread_mutex_init(&verrou, NULL);

    for(int i=1; i<=5; i++){
        pthread_mutex_lock(&verrou);
        if (pthread_create(&thr, NULL, thread_function, i) != 0) {
            fprintf(stderr, "Error during pthread_create()\n");
            exit(EXIT_FAILURE);
        }

        pthread_join(&thr, NULL);
        pthread_mutex_unlock(&verrou);
    }


    while (1) {
        fprintf(stderr, "Main thread\n");
        sleep(5);
    }
    pthread_mutex_destroy(&verrou);
}
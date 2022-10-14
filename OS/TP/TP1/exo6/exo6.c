#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main () {
    char buf [80];
    printf ("[%d]-- Debut du test exec () \n",getpid());
    sprintf (buf,"-- pid=[%d]", getpid());
    execl ("/bin/ech","echo","Execution","d'un test exec",buf, NULL);
    printf ("[%d]-- Echec de la fonction execl () \n",getpid());
    printf ("[%d]-- Fin du test exec () \n",getpid());
    exit(0);
}
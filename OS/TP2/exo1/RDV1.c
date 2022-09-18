#include <sys/types.h>     /* key_t  */
#include <sys/ipc.h>       /* ftok   */
#include <sys/sem.h>       /* semget, semctl, semop */
#include <sys/stat.h>      /* stat, struct stat  */
#include <stdlib.h>        /* malloc */
#include <stdio.h>         /* perror, printf */
#include <errno.h>         /* errno */
#include <unistd.h> 	   /*sleep*/

#define PRJVAL  1

// Ce programme doit �tre dupliqu� pour chaque processus voulant �tablir un RDV
// vous pouvez nommer le programme du premier processus RDV1.C et celui du 2nd RDV2.C par exemple
// Les 2 programmes doivent �tre compil�s s�parement et ex�cuter s�parement sur le shell


/* retourne -1 en cas d'erreur           */
int P(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    // Q- donner les 3 �l�ments de la structure Ops pour r�aliser l'op�ration (voir le cours)
    // Ops[0].sem_num = ...; ...
    Ops[0].sem_num = noSem;
    Ops[0].sem_op = -1;
    Ops[0].sem_flg = 0;

    // Q- faire appel � la fonction semop pour r�aliser l'op�ration P, la variable OK r�cup�re la valeur de retour
    ok = semop(semid, Ops, 1);

    if((ok = semop(semid, Ops, 1))) return ok;
    else return -1;
}

/* retourne -1 en cas d'erreur           */
int V(int semid, int noSem)
{
    struct sembuf Ops[1];
    int ok;

    // Q- donner les 3 �l�ments de la structure Ops pour r�aliser l'op�ration (voir le cours)
    // Ops[0].sem_num = ...; ...
    Ops[0].sem_num = noSem;
    Ops[0].sem_op = +1;
    Ops[0].sem_flg = 0;

    // Q- faire appel � la fonction semop pour r�aliser l'op�ration V, la variable OK r�cup�re la valeur de retour
    ok = semop(semid, Ops, 1);

    if((ok = semop(semid, Ops, 1))) return ok;
    else return -1;
}

int main (void)
{ // � compl�ter sans oublier de supprimer l'ensemble des s�maphores

    // Q- Il faut d'abord recr�er la cl� (voir sema.c)
    key_t k;
    k = ftok("toto", PRJVAL);

    // Q- il faut ensuite ouvrir le semaphore avec semget, � part la cl�, les autres argument doivent �tre � z�ro
    // car il ne s'agit pas d'une cr�ation mais d'une ouverture
    int semid = semget(k, 0, 0);
    printf("cle %d (dec) %x (hex)\n", k, k);
    printf("semid obtenu:%d\n", semid);

    // Q- faire l'appel � sleep() afin d'avoir des attentes de diff�rentes dur�es pour les 2 processus
    sleep(1);

    // Q- faire appel � P et � V (voir le TD)
    V(semid, 1);
    P(semid, 0);

    // appeler la fonction de RDV, un printf est suffisant.
    printf("RDV1");

    semctl(semid, 2, IPC_RMID);
    return 0;
}
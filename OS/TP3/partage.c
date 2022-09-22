//
// Created by Tanguy Roudaut on 21/09/2022.
//

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
    if ((zp->id=shmget(IPC_PRIVATE, taille, 0600|IPC_CREAT))==-1)
    {
        perror("shmget (creerZonePartagee)");
        return -1;
    }
    if ((zp->debut=shmat(zp->id, 0, 0))==(void*)-1)
    {
        perror("shmat (creerZonePartagee)");
        shmctl(zp->id, IPC_RMID, NULL);
        return -1; }
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
    if (shmdt(zp->debut)==-1)
    { perror("shmdt (supprimerZonePartagee)"); return -1; }
    if (shmctl(zp->id, IPC_RMID, NULL)==-1)
    { perror("shmctl (supprimerZonePartagee)"); return -1; }
    return 0;
}

void writeZonePartagee(Zone * zp, int nbChar){
    zp->taille = nbChar;
}

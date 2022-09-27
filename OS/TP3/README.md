---
title: "TP3 - Communication Inter-Processus"
date: 21 septembre 2022
---

# Table des matières

- [La mémoire partagé](#la-mémoire-partagé)
  - [Les fonctions de la mémoire partagé](#les-fonctions-de-la-mémoire-partagé)
  - [Utilisation de la mémoire partagé](#utilisation-de-la-mémoire-partagé)
- [Les tubes](#les-tubes)
  - [Les fonctions des tubes](#les-fonctions-des-tubes)
  - [Utilisation des tubes](#utilisation-des-tubes)
- [Les signaux](#les-signaux)
    - [Les fonctions des signaux](#les-fonctions-des-signaux)
    - [Utilisation des signaux](#utilisation-des-signaux)
- [Le code du TP](#le-code-du-tp)


# La mémoire partagé
## Les fonctions de la mémoire partagé
   1. **La fonction ```shmget()```** :
   2. **La fonction ```shmat()```** :
   3. **La fonction ```shmctl()```** :
   4. **La fonction ```shmdt()```** :

## Utilisation de la mémoire partagé
   - Exemple :

      ```c
      //TODO
      ```


# Les tubes
## Les fonctions des tubes
   1. **La fonction ```pipe()```** :

## Utilisation des tubes
   - Exemple :

      ```c
      //TODO
      ```

# Les signaux
## Les fonctions des signaux
   1. **La fonction ```signal()```** :
   2. **La fonction ```sigaction()```** :
   3. **La structure ```struct sigaction``` avec la fonction ```sigaction()```** :
   4. **La fonction ```kill()```** :
   5. **La fonction ```pause()```** :

## Utilisation des signaux
   - Exemple :

      ```c
      //TODO
      ```
  
# Le code du TP
```c
/** nbOctets.c **/

```

```c
/** partage.h **/

#ifndef DEPOT_ENSTA_C_PARTAGE_H
#define DEPOT_ENSTA_C_PARTAGE_H

/* Structure zone representant une zone partagee entre processus ayant un
lien de parente. Exemple: pere-fils => cle privee d’IPC Sys V */
typedef struct
{
  int id;
  void * debut;
  int taille;
} Zone;

/* fonctions publiques */
extern int creerZonePartagee(int taille, Zone * zp);
extern int supprimerZonePartagee(Zone * zp);

#endif //DEPOT_ENSTA_C_PARTAGE_H
```

```c
/** partage.c **/

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
  if ((zp->id=shmget(IPC_PRIVATE, taille, 0600|IPC_CREAT))==-1){
    perror("shmget (creerZonePartagee)");
    return -1;
  }
  if ((zp->debut=shmat(zp->id, 0, 0))==(void*)-1){
    perror("shmat (creerZonePartagee)");
    shmctl(zp->id, IPC_RMID, NULL);
    return -1; 
  }
  
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
  if (shmdt(zp->debut)==-1){ 
      perror("shmdt (supprimerZonePartagee)"); 
      return -1; 
  }
  if (shmctl(zp->id, IPC_RMID, NULL)==-1){ 
      perror("shmctl (supprimerZonePartagee)"); 
      return -1; 
  }
  
  return 0;
}
```



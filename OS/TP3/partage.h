//
// Created by Tanguy Roudaut on 21/09/2022.
//
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


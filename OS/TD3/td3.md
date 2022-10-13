# TD3 - Communication inter-processus

## Exercice 1
---
```c
void p1(){
    sleep(1);
    envoie(p2, DEMANDE_RDV);
    reception(p2, ACCUSE_RECEPTION);
    rdv1();
}

void p2(){ // c'est ce processus qui gére le rdv le confirme
    sleep(1);
    reception(p2, DEMANDE_RDV);
    envoie(p2, ACCUSE_RECEPTION);
    rdv2();
}
```

## Exercice 2
---
```c
void pi(){
    int nbArrive = 0;

    sleep(i);

    if(i<N){
        envoie(P[N], DEMANDE_RDV);
        reception(P[N], ACCUSE_RECEPTION);
    }
    else { // C'est le processus N qui gére les rdv
        while(nbArrive < N-1){
            reception(NULL, DEMANDE_RDV);
            nbArrive ++;
        }

        for(int j=0; i<N-1; j++){
            envoie(P[j], ACCUSE_RECEPTION);
        }
    }
}
```
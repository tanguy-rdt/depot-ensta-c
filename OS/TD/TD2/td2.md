# TD2 - Exclusion mutuelle/Sémaphores


## Exercice 1:

```c
rdv1, rdv2 = 0;

void P1(){
    sleep(1);
    V(rdv1);
    P(rdv2);
    rdv1();
}

void P2(){
    sleep(5);
    V(rdv2);
    P(rdv1);
    rdv2();
}
```

## Exercice 2:

```c
rdv1, rdv2, rdv3 = 0;

void P1(){
    sleep(1);
    V(rdv1);
    V(rdv1);
    P(rdv2);
    P(rdv3);
    rdv1();
}

void P2(){
    sleep(5);
    V(rdv2);
    V(rdv2);
    P(rdv1);
    P(rdv3);
    rdv2();
}

void P3(){
    sleep(10);
    V(rdv3);
    V(rdv3);
    P(rdv1);
    P(rdv2);
    rdv3();
}
```

## Exercice 3:
---

```c
nProb = N;
sema[N];

void semaInit(int N){
    for(int i=0; i<N; i++){
        sema[i] = 0;
    }
}


Px(int x, int N){
    for(int i=0; i<N; i++){
        V(sema[i]);
    }
    for(int i=0; i<N; i++){
        if( i != x) P(i);
    }

    rdvx();
}
```
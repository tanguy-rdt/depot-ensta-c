# Ce qu'il faut retenir du TP1

## Table des matières
1. [La primitive *fork()*](#la-primitive-fork)
2. [Caractéristique générale d'un processus](#caractristique-gnrale-dun-processus)
3. [La primitive *exit()* et *wait()*](#la-primitive-exit-et-wait)
4. [La primitive *waitpid()*](#la-primitive-waitpid)
5. [Recouvrement d'un processus (*execl* et *execv*)](#recouvrement-dun-processus-execl-et-execv)

## La primitive *fork()* 
*cf.* [*exo1*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo1/exo1.c)

La primitive *fork()* permet de créer un nouveau processus (processus fils) paralèllement a celui qui éxecute le code (processus père).

- ***Exemple :***
    ```c
    #include <stdio.h>
    #include <unistd.h>
    
    int main(){
        int pid = fork(); 
    
        if (pid == -1)
            printf("Unable to create the process\n");
        else if (pid == 0)
            printf("[%d] -- I'm the son process\n", getpid());
        else
            printf("[%d] -- I'm the father process\n", getpid());
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process

## Caractéristique générale d'un processus
*cf.* [*exo2*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo2/exo2.c) *&* [*exo3*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo3/exo3.c)

- *PID* du processus actuel :
    ```c
    #include <unistd.h>
    
    pid = getpid();
    ```

- Propriétaire réel et effectif :
    ```c
    #include <unistd.h>
    
    realOwner = getuid();
    beneficialOwner = geteuid();
    ```

- Le groupe propriétaire réel et effectif :
    ```c
    #include <unistd.h>
    
    realOwnerGroup = getgid();
    beneficialOwnerGroup = getegid();
    ```
  
- Répertoire de travail :
    ```c
    #include <unistd.h>
    
    char processusPath[100];
    getcwd(processusPath, sizeof(processusPath));    
    ```

## La primitive *exit()* et *wait()* 
*cf.* [*exo4*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo4/exo4.c)

- ***Exemple :***
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    
    int main(){
        int pid = fork(); 
        int status = 0;
    
        if (pid == -1){
            printf("Unable to create the process\n");
        }
        else if (pid == 0){
            sleep(5);
            printf("[%d] -- I'm the son process\n", getpid());
            exit(0);
        }
        else {
            printf("[%d] -- I'm the father process\n", getpid());
            wait(&status);
        }
        
        printf("exit code of the son process: %d", status);
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process <br>
      exit code of the son process: 0

## La primitive *waitpid()*
*cf.* [*exo5*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo5/exo5.c)

- ***Exemple :***
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>
    
    int main(){
        int pid = fork(); 
        int terminaison = 0;
        int retWaitPid = 0;
    
        if (pid == -1){
            printf("Unable to create the process\n");
        }
        else if (pid == 0){
            sleep(5);
            printf("[%d] -- I'm the son process\n", getpid());
            exit(0);
        }
        else {
            printf("[%d] -- I'm the father process\n", getpid());
            retWaitPid = waitpid(pidFils, &terminaison, 0);
        }
        
        printf("PID of the zombie son process: %d\n", retWaitPid);
        
        if (WIFEXITED(terminaison))
            printf("Return code of the child process: %d\n", WEXITSTATUS(terminaison));
        else if (WIFSIGNALED(terminaison))
            printf("ERROR: Number of the signal that caused the process to end: %d\n", WTERMSIG(terminaison));
    }
    ```
  
    > [3734] -- I'm the father process <br>
      [3735] -- I'm the son process <br>
      PID of the zombie son process: 3735 <br>
      Return code of the child process: 0

## Recouvrement d'un processus (*execl* et *execv*)
*cf.* [*exo6*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo6/exo6.c) *&* [*exo7*](https://git.roudaut.xyz/ensta/depot-ensta-c/-/blob/SA3/OS/TP1/exo7/exo7.c)

- ***Exemple avec execl:***
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    
    int main () {
        char buf [80];
  
        printf ("[%d]-- Start of the exec() test\n",getpid());
        sprintf (buf,"-- pid=[%d]", getpid());
        execl ("/bin/echo","echo","Execution","of an exec test",buf, NULL);
        printf ("[%d]-- Failure of the execl() function\n",getpid());
        printf ("[%d]-- End of exec() test\n",getpid());
        exit(0);
    }
    ```

  > [4683]-- Start of the exec() test <br>
  Execution of an exec test -- pid=[4683]




- ***Exemple avec execv:***
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    
    int main () {
        char buf [80];
        char *argv [5];
        argv [0] = "echo";
        argv [1] = "Execution";
        argv [2] = "of an exec test";
        argv [3] = buf;
        argv [4] = NULL;
    
        printf ("[%d]-- Start of the exec() test\n",getpid());
        sprintf (buf,"-- pid=[%d]", getpid());
        execv ("/bin/echo", argv);
        printf ("[%d]-- Failure of the execv() function\n",getpid());
        printf ("[%d]-- End of exec() test\n",getpid());
        exit(0);
    }
    ```

  > [4712]-- Start of the exec() test <br>
  Execution of an exec test -- pid=[4712]

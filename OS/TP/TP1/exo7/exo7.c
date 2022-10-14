#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
	int pidFils = fork();
    char processusPath[256];
    int terminaison = 0;
    int retWaitPid = 0;
    char *argv [3];

	switch(pidFils){
		case -1:
			printf("Pb lors de la création du processus\n");
			break;

		case 0:
            argv [0] = "ps";
            argv [1] = "-l";
            argv [2] = NULL;

            printf ("[%d]-- Debut du test 'ps -l' \n",getpid());
            execv ("/bin/ps", argv);
            printf ("[%d]-- Echec de la cmd 'ps -l' \n",getpid());
            printf ("[%d]-- Fin du test 'ps -l' \n",getpid());
            exit(-1);
			break;

		default:
            retWaitPid = waitpid(pidFils, &terminaison, 0); // WNOHANG permet au processus appelant de ne pas être bloqué si le processus demandé n’est pas terminé
            break;
	}

    printf("Pid du processus fils zombi: %d\n", pidFils);
    printf("Code retour du waitpid: %d\n", retWaitPid);

    if (WIFEXITED(terminaison))
        printf("Code retour du processus fils: %d\n", WEXITSTATUS(terminaison));
    else if (WIFSIGNALED(terminaison))
        printf("ERREUR: Numéro du signal ayant provoqué la fin du processus: %d\n", WTERMSIG(terminaison));


    
}
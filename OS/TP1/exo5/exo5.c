#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
	printf("Debut du test fork()\n");
	int pidFils = fork();
    char processusPath[256];
    int terminaison = 0;
    int retWaitPid = 0;

	switch(pidFils){
		case -1:
			printf("Pb lors de la création du processus\n");
			break;

		case 0:
            sleep(10);
			printf("\nCaractéristique du processus fils\n");
            printf("\tPID --> %d\n", getpid());
            printf("\tPID du père --> %d\n", getppid());
            printf("\tRépertoire de travail du processus --> %s\n", getcwd(processusPath, sizeof(processusPath)));
            printf("\tPropriétaire réel --> %d\n", getuid());
            printf("\tPropriétaire effectif --> %d\n", geteuid());
            printf("\tLe groupe propriétaire réel --> %d\n", getgid());
            printf("\tLe groupe propriétaire effectif --> %d\n", getegid());
            exit(3);
			break;

		default:
			printf("\nCaractéristique du processus père\n");
            printf("\tPID --> %d\n", getpid());
            printf("\tPID du fils créé --> %d\n", pidFils);
            printf("\tRépertoire de travail du processus --> %s\n", getcwd(processusPath, sizeof(processusPath)));
            printf("\tPropriétaire réel --> %d\n", getuid());
            printf("\tPropriétaire effectif --> %d\n", geteuid());
            printf("\tLe groupe propriétaire réel --> %d\n", getgid());
            printf("\tLe groupe propriétaire effectif --> %d\n", getegid());
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
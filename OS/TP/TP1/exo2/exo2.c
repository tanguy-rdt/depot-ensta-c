#include <stdio.h>
#include <unistd.h>

int main(){
	int code_retour; 
	printf("Debut du test fork()\n");
	code_retour = fork();
    char processusPath[100];


	switch(code_retour){
		case -1:
			printf("Pb lors de la création du processus\n");
			break;

		case 0:
			printf("\nCaractéristique du processus fils\n");
            printf("\tPID --> %d\n", getpid());
            printf("\tPID du père --> %d\n", getppid());
            printf("\tRépertoire de travail du processus --> %s\n", getcwd(processusPath, sizeof(processusPath)));
            printf("\tPropriétaire réel --> %d\n", getuid());
            printf("\tPropriétaire effectif --> %d\n", geteuid());
            printf("\tLe groupe propriétaire réel --> %d\n", getgid());
            printf("\tLe groupe propriétaire effectif --> %d\n", getegid());
			break;

		default:
			printf("\nCaractéristique du processus père\n");
            printf("\tPID --> %d\n", getpid());
            printf("\tRépertoire de travail du processus --> %s\n", getcwd(processusPath, sizeof(processusPath)));
            printf("\tPropriétaire réel --> %d\n", getuid());
            printf("\tPropriétaire effectif --> %d\n", geteuid());
            printf("\tLe groupe propriétaire réel --> %d\n", getgid());
            printf("\tLe groupe propriétaire effectif --> %d\n", getegid());
            break;
	}
}
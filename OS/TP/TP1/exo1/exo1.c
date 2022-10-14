#include <stdio.h>
#include <unistd.h>

int main(){
	int code_retour; 
	printf("Debut du test fork()\n");
	code_retour = fork();

	switch(code_retour){
		case -1:
			printf("Pb lors de lq création du processus\n");
			break;

		case 0:
			printf("Je suis le processus fils\n");
			break;

		default:
			printf("Je suis le processus père\n");
			printf("Je viens de créer le processus fils dont le pid est %d\n", code_retour);
	}

	printf("code_retour %d\n", code_retour);
	printf("Fin du test fork()\n");
}
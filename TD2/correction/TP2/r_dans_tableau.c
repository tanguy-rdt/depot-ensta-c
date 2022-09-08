#include <stdio.h>

int main(int argc, char* argv[]){
    // on met moins de 100 char dedans, pas grave
    
    // avec des 'r'
    //char array_to_search[100] = "qjkdfhkqsfhklqsdhflqsdhfkjqhsdfkhqsdkf<kjhrrrioqfqdshrrjqhaor"; 

    // sans 'r', en choisir un des deux à la compilation
    char array_to_search[100] = "qldshbqsdcbkqdcbqsbdcqbn,bkqlclmdkjlmqjlqdcndqsn,f,dvbksdqghf";

    int nb_r = 0;
    for (int i=0; i < 100; i++){
        if (array_to_search[i] == 'r'){
            nb_r++;
        }
    }

    if (nb_r == 0){
        printf("Il n'y a pas de r\n");
    } else {
        printf("Il y a %d 'r'\n", nb_r);
    }
    return 0;
}

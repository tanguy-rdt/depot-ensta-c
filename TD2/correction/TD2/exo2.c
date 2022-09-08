#include <stdio.h>


int main(int argc, char* argv[]){
    int array_to_sum[] = {1, 2, 3, 4, -5, 6, 7, 8, 9};

    int sum_all = 0, sum_pos = 0, sum_short = 0;

    for (int i = 0; i < 9; i++){ //attention si on rate le "9" on additionne n'importe quoi
        sum_all += array_to_sum[i];

        if (array_to_sum[i] >= 0){
            sum_pos += array_to_sum[i];
        } 
    } // for (int i...


    /*
    for (int i = 0; array_to_sum[i] >= 0; i++){
        sum_short += array_to_sum[i];
    }
    */

    sum_short = 0;
    int i = 0;
    while (i < 9 && array_to_sum[i] >= 0){ // attention à vérifier d'abord si i pas trop grand
        sum_short += array_to_sum[i];
        i++;
    }
    

    printf("La somme de tous les entiers est: %d\n", sum_all);
    printf("La somme de tous les entiers positifs est: %d\n", sum_pos);
    printf("La somme tronquée aux premiers entiers positifs est: %d\n", sum_short);
    return 0;

}

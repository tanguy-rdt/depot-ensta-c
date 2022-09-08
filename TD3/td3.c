//
// Created by Tanguy Roudaut on 05/07/2022.


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//Transtypage
/*
#define NB_CHAR 16
#define NB_INT 4
#define NB_DOUBLE 2


typedef struct sensor_data_t {
    char text[NB_CHAR];
    uint16_t int_values[NB_INT];
    double double_values[NB_DOUBLE];
};

int main(){
    uint8_t data[] = "\x44\x61\x74\x61\x20\x72\x65\x63\x65\x69\x76\x65\x64\x3a\x20\x20\t\x00\n\x00*\x00\x08\x00\x18-DT\xfb!\t@iW\x14\x8b\n\xbf\x05@";

    char* pChar = (char*)data;
    uint16_t* pInt = (uint16_t*)(pChar + NB_CHAR);
    double* pDouble = (double*)(pInt + NB_INT);

    char valChar[NB_CHAR];
    uint16_t valInt[NB_INT];
    double valDouble[NB_DOUBLE];

    for (int i = 0; i < NB_CHAR; i++){
            valChar[i] = pChar[i];
            printf("%c", valChar[i]);
    }

    for (int i = 0; i < NB_INT; i++){
        valInt[i] = pInt[i];
        printf("%d ", valInt[i]);
    }

    for (int i = 0; i < NB_DOUBLE; i++){
        valDouble[i] = pDouble[i];
        printf("%f ", valDouble[i]);
    }

    sensor_data_t* new_data;
    new_data = (sensor_data_t*)data;

    printf("%p %p %p %ld %ld\n", new_data->text, new_data->int_values, new_data->double_values,
           (char*)new_data->int_values - new_data->text, (uint16_t*)new_data->double_values - new_data->int_values);


    for(int i=0; i<NB_CHAR; i++){
        printf("%c", new_data->text[i]);
    }

    for(int i=0; i<NB_INT; i++){
        printf("%hu ", new_data->int_values[i]); // %hu comme entier non signe su 2 octet, %d marche pour entier standard
    }

    for (int i=0; i<NB_DOUBLE; i++){
        printf("%lf ", new_data->double_values[i]);
    }
    printf("\n");



    return 0;
}*/


//array decay
/*
#define STR_LEN   9 // 8+1
#define ARRAY_LEN   4

// 1/ lire le main, compiler le programme, ignorer le warning à la compilation
// 2/ expliquer ce qu'affichent les trois printf de print_array en utilisant sa
// troisième définition ne pas hésiter à lancer le programme plusieurs fois
// pour voir ce qui change
// 3/ pourquoi le programme plante-t-il ?

//void print_array(int array_len, int str_len, char to_print[][str_len]);
void print_array(int array_len, int str_len, char (*to_print)[str_len]);
//void print_array(int array_len, int str_len, char **to_print);

int main(int agrc, char *argv[]){

	// Est-ce qu'un tableau est vraiment parfaitement équivalent à un
	// pointeur ? On a vu que dans le cas d'un tableau à une dimension
	// cette approximation fonctionne, cependant on peut observer un
	// comportement surprenant avec sizeof() et &.

	char two_words[] = "Guten Tag";
	printf("two_words: %p, &two_words %p\n", two_words, &two_words);
	printf("two_words + 1: %p, &two_words + 1 %p\n", two_words + 1, &two_words + 1);
	printf("sizeof(two_words) %lu, sizeof(&two_words) %lu\n", sizeof(two_words), sizeof(&two_words));

	// Dans le cas de sizeof et &, two_words est vu pour ce qu'il est
	// vraiment: un tableau de 10 caractères dont on va respectivement
	// chercher la taille (10) et l'adresse. On a donc bien l'adresse
	// du tableau de 10 caractères qui est la même que le pointeur
	// vers ces 10 mêmes caractères (heureusement !), d'où le comportement
	// de &. Quand on fait "two_words + 1" on incrémente de 1 un pointeur
	// de caractères, donc on avance d'un octet. Par contre quand on fait
	// &two_words + 1 on incrémente de 1 l'adresse d'un tableau de 10
	// caractères, donc on avance de 10 octets.
	// Dans toutes les autres utilisations il est vu comme un pointeur
	// vers des char, soit un char*. Ce comportement s'appelle
	// "array decay" ou en plus long "array to pointer decay" qu'on
	// pourrait tenter de traduire en "désintégration de tableaux vers
	// pointeur".
	// Si dans le cas des tableaux mono-dimensionnels on peut se contenter
	// de dire "c'est pareil", ce n'est pas du tout le cas pour les tableaux
	// de dimensions supérieures comme on va l'illustrer avec print_array
	// ci-dessous.

	char words_array[][STR_LEN] = {"Bonjour1", "Bonjour2", "Bonjour3", "Bonjour4"};
    print_array(ARRAY_LEN, STR_LEN, words_array);
	return 0;
}

//void print_array(int array_len, int str_len, char to_print[][str_len]){
void print_array(int array_len, int str_len, char (*to_print)[str_len]){
//void print_array(int array_len, int str_len, char **to_print){
    printf("%p\n", to_print);
    printf("%p\n", to_print[0]);
    printf("%p\n", &to_print[0]);
    for(int i = 0; i < array_len; i++){
        printf("%s\n", to_print[i]);
    }
}*/

//TD3/TP3

int average_int(int val1, int val2);
double average_double(double val1, double val2);
int average_int_array(int tabInt[], int length);
int average_int2(int val1, int val2);
long average_long_array(long* tabInt[], int length);
long average_arg(char* val[], int length);

int main(int argc, char* argv[]){
    int averageInt = average_int(5, 10);
    double averageDouble = average_double(5.10, 20.50);

    int valInt[10] = {1, 2 , 3, 4, 0, 6, 7, 8, 9, 0};
    int averageTableInt = average_int_array(valInt, 10);

    int averageInt2 = average_int2(5, 20);

    int averageArg = average_arg(argv, argc - 1);

    printf("moyenne de 5 et 10: %d \n", averageInt);
    printf("moyenne de 5.10 et 20.50: %f \n", averageDouble);
    printf("moyenne du tableau: %d \n", averageTableInt);
    printf("moyenne de 5 et 20: %d \n", averageInt2);
    printf("moyenne des arguments: %d \n", averageArg);
}

int average_int(int val1, int val2){
    return (val1 + val2)/2;
}

double average_double(double val1, double val2){
    return (val1 + val2)/2;
}

int average_int_array(int tabInt[], int length){
    int sum = 0;

    for (int i = 0; i < length; i++){
        sum += tabInt[i];
    }

    return sum/length;
}

int average_int2(int val1, int val2){
    int tableInt[2] = {val1, val2};
    return average_int_array(&tableInt[0], 2);
}

long average_long_array(long* tabInt[], int length){
    long sum = 0;

    long* tab = *tabInt;

    for (int i = 0; i < length; i++){
        sum += tab[i];
    }

    return sum/length;
}

long average_arg(char** val, int length){
    if (length == 0){
        printf("Aucun argument\n\n");
        return -1;
    }

    long* tabVal;
    tabVal = malloc(sizeof(long)* length);
    if (tabVal == NULL){
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < length; i++){
        tabVal[i] = strtol(val[i + 1], NULL, 10);
        /* // SOLUTION BOURAIN
        errno = 0;
        tab[i] = strtol(argv[i + 1], &endptr, 10);
        if ((errno == ERANGE && (tab[i] == LONG_MAX || tab[i] == LONG_MIN))
                        || (errno != 0 && tab[i] == 0)) {
            perror("strtol");
            exit(EXIT_FAILURE);
        }

        if (endptr == argv[i + 1]) {
            fprintf(stderr, "Pas de chiffre trouvé en %dème position\n", i + 1);
            exit(EXIT_FAILURE);
        }
        */
    }

    return average_long_array(&tabVal, length);
}






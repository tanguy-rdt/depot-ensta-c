#include <stdio.h>

// vous pouvez vous contenter de int main(void) pour le moment
int main(int argc, char* argv[]){
    int a; //on declare a sans l'initialiser
    int b = 3.0; //on déclare b et on l'initialise
    double pi = 3.14; //on déclare b et on l'initialise
    a = 10; //on initialise a
    int c = a/b;
    int toto;
    double d = a/(double)b;
    const char tab[] = "Coucou";

    // on a x = 1 et y = 1 arrivé ici
    int z = 2 >= 3 && 2; // je vote 0
    int x = 1 == 3 || 2; // je vote 1
    int y = x == z; // je vote 0
    
    printf("%d %d %d\n", x, y, z);

    //en python double(d)

    printf("a vaut %d, b vaut %d, la division de a par b vaut %d en double ça vaut %lf\n", a, b, c, d);
    printf("a divisé par b vaut %d\n", a/b);
    printf("pi vaut %lf\n", pi);
    printf("toto vaut %d\n", toto);

    printf("%d\n", 4 - 3 * 2 - 1);
    printf("%d\n", 2+3*6+7*2*-2/4);
    printf("%d\n", 8/4 + 2*10%5 + 3 + 2/1);
    return 0;
}

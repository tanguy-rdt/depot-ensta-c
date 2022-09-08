//
// Created by Tanguy Roudaut on 07/07/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _student
{
    // le contenu de la structure
    char *name;
    int age;

    // où trouver la suite
    struct _student* next;
} student_t;

void displayList(student_t studentList);
int nbStudent(student_t studentList, student_t** ptrLastStudent);
void deleteStudent(student_t* ptrCurrentStudent, char name[]);
int createStudent(student_t* ptrCurrentStudent, char name[], int age);
int createsStudents(student_t* ptrCurrentStudent, char* names[], int ages[], int nbStudents);
int insertStudent(student_t* ptrCurrentStudent, char names[], int age, int position);

student_t createList();

int main(int argc, char* argv[]){
    char *names[] = {"Foo", "Bar", "Baz"};
    int ages[] = {42, 45, 49};


    student_t student_list = createList();

    //createStudent(&student_list, "Foo", 42);
    //createStudent(&student_list, "Bar", 45);
    //createStudent(&student_list, "Baz", 49);

    createsStudents(&student_list, names, ages, 3);

    printf("La liste et la suivante: \n");
    displayList(student_list);
    printf("\n");

    printf("Ajout de la personne toto qui a 70 ans en position 2: \n");
    insertStudent(&student_list, "toto", 70, 2);
    displayList(student_list);
    printf("\n");

    printf("Suppression de la personne Bar:\n");
    deleteStudent(&student_list, "Bar");
    displayList(student_list);
    printf("\n");

    student_t* ptrLastStudent;
    int nbStudents = nbStudent(student_list, &ptrLastStudent);
    printf("Il y a %d étudiants, le dernier est %s \n", nbStudents, ptrLastStudent -> name);

    return 0;
}

student_t createList(){
    student_t studentList;
    studentList.name = NULL;
    studentList.age = -1;
    studentList.next = NULL;

    return studentList;
}

void displayList(student_t studentList){
    student_t *ptrStudent;
    ptrStudent = studentList.next;

    while(ptrStudent){
        printf("%s %d\n", ptrStudent -> name, ptrStudent -> age);
        ptrStudent = ptrStudent -> next;
    }
}

int nbStudent(student_t studentList, student_t** ptrLastStudent){
    student_t* ptrCurrentStudent = &studentList;

    int nbStudents = 0;
    while(ptrCurrentStudent -> next){
        nbStudents++;
        ptrCurrentStudent = ptrCurrentStudent -> next;
    }

    *ptrLastStudent = ptrCurrentStudent;

    return nbStudents;
}

void deleteStudent(student_t* ptrCurrentStudent, char name[]){
    while(ptrCurrentStudent){

        if (ptrCurrentStudent -> next && !strncmp(ptrCurrentStudent -> next -> name , "Bar", 3)){
            student_t *student_to_delete = ptrCurrentStudent -> next;
            ptrCurrentStudent -> next  = student_to_delete -> next;
            free(student_to_delete);
        }

        ptrCurrentStudent = ptrCurrentStudent -> next;
    }
}

int createStudent(student_t *ptrCurrentStudent, char name[], int age){
    if(strlen(name) == 0 || age <= 0) {
        return 0;
    }

    while (ptrCurrentStudent -> next){ // le prob est qu'on parcours à chaque fois tous les éléments de la liste
        ptrCurrentStudent = ptrCurrentStudent -> next;
    }

    ptrCurrentStudent -> next = (student_t*)malloc(sizeof(student_t)); // allocation mémoire d'un nouvel étudiant
    ptrCurrentStudent = ptrCurrentStudent -> next; // déplacament du pointeur de l'étudiant courant vers l'étudiant créé
    ptrCurrentStudent -> name = name;
    ptrCurrentStudent -> age = age;
    ptrCurrentStudent -> next = NULL;// pour l'instant c'est le dernier étudiant

    return 1;
}

int createsStudents(student_t *ptrCurrentStudent, char* names[], int ages[], int nbStudents){
    int ret = 0;
    for (int i = 0; i < nbStudents; i++){
        ret = createStudent(ptrCurrentStudent, names[i], ages[i]);

        if (!ret){
            return ret;
        }
    }

    return ret;
}

int insertStudent(student_t *ptrCurrentStudent, char name[], int age, int position){
    for(int i = 0; i < position - 1; i++){ // si la position est 5 on va jusqu'a 4
        if(ptrCurrentStudent -> next){
            ptrCurrentStudent = ptrCurrentStudent -> next; // on incrémente le l'étudiant
        }
        else {
            return -1; // position trop grande par rapport à la longueur de la liste
        }
    }

    student_t* lastNextStudent = ptrCurrentStudent -> next; // on enregistre l'étudiant en 5 position

    ptrCurrentStudent -> next = (student_t*)malloc(sizeof(student_t)); // on alloue une taille pour un nouvelle étudiant en 5ème position
    ptrCurrentStudent = ptrCurrentStudent -> next; // on passe de la 4 ème à 5ème position
    ptrCurrentStudent -> name = name;
    ptrCurrentStudent -> age = age;
    ptrCurrentStudent -> next = lastNextStudent; // on dit que l'étudiant suivant au 5 ème est l'ancien 5 ème.
}



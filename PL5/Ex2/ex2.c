#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

/**
 * 2. Consider an array, in which each array element is a structure. 
 * The fields of such structure are: number, name and grade. 
 * The array has a capacity for 5 elements. Implement a function to 
 * receive as parameter one element of the array and print it. 
 * The function must print all the fields of the structure. 
 * Assumptions:
 * • Create 5 threads;
 * • Each thread must start in the created function;
 * • Each thread will only deal with an element of the array.
 */

#define CHAR_SIZE 50
#define ARR_SIZE 5

/**
 * structure : number, name and grade
 */
struct estrutura{

    int number;
    char name[CHAR_SIZE];
    float grade;
};

void* thread_func(void *element){

    struct estrutura value = *((struct estrutura *) element);

    printf("Num: %d\nName: %s\nGrade: %.2f\n", value.number, value.name, value.grade);
    pthread_exit(NULL);
}

int main(){

    struct estrutura strArr[ARR_SIZE];
    pthread_t threads[5];
    int i, tr;

    /* dar valores ao array */

    strArr[0].number = 0;
    strcpy(strArr[0].name, "Ana");
    strArr[0].grade = 12;

    strArr[1].number = 1;
    strcpy(strArr[1].name, "Daniela");
    strArr[1].grade = 14.9;

    strArr[2].number = 2;
    strcpy(strArr[2].name, "Isabel");
    strArr[2].grade = 16;

    strArr[3].number = 3;
    strcpy(strArr[3].name, "Raquel");
    strArr[3].grade = 15.5;

    strArr[4].number = 4;
    strcpy(strArr[4].name, "José");
    strArr[4].grade = 13;

    for(i = 0; i< 5; i++){

        tr = pthread_create((&threads[i]), NULL, thread_func, &(strArr[i]));

        if(tr < 0){
            printf("Thread creation error %d", i);
        } else {
            printf("Thread: %d\n", i);
        }

        /* espera que a determinada thread acabe */
        pthread_join(threads[i], NULL);
    }


    return 0;
}
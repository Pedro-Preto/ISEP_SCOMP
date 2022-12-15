/**
 * @file reader.c
 * @author Ana Barreiro
 *
 *  The reader should read the 100 values, calculate the sum of ASCII code
 * and print all the chars and the average of the 100 ASCII codes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define ARR_SIZE 100

typedef struct {

    char arr[ARR_SIZE];

} randomValues;

int main(){

    randomValues *ptr_randomValues;
    int shm, i, st_size = sizeof(randomValues), sum = 0, average;

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((shm = shm_open("/shmex04", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

        perror("Failed to create shared memory");
    }

    /*
     * ftruncate para ajustar o tamanho da struct à memoria partilhada
     */
    if(ftruncate(shm, st_size) < 0){

        perror("Failed to adjust memory size");
    }

    /*
    * mapear a memoria partilhada no nosso programa
    */
    ptr_randomValues = (randomValues *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if (ptr_randomValues == NULL) {
        perror("No mmap()");
        exit(0);
    }

    /**
     * Calcula a soma dos chars (ASCII code) e da print nos valores aleatórios
     */

    for (i = 0; i < ARR_SIZE; i++){

        sum += ptr_randomValues -> arr[i];
        printf("Char: %c\n", ptr_randomValues -> arr[i]);
    }

    printf("Sum: %d\n", sum);

    /**
     * Calcula e da print na média de ASCII code dos chars
     */
    average = sum / ARR_SIZE;
    printf("Average: %d\n", average);

    /*
    * Desfaz o mapeamento
    */
    if(munmap(ptr_randomValues, st_size) < 0){

        perror("No munmap()");
        exit(0);
    }

    /*
     * Fecha o descritor
     */
    if(close(shm) < 0){

        perror("No close()");
        exit(1);
    }

    return 0;
}

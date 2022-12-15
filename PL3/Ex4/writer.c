/**
 * @file writer.c
 * @author Ana Barreiro
 *
 *  The writer must create a shared memory area, generate 100 random chars
 * (between ‘A’ and ‘Z’) and write them in the shared memory
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define ARR_SIZE 100

typedef struct {

    char arr[ARR_SIZE];

} randomValues;

int main(){

    randomValues *ptr_randomValues;
    int shm, i, st_size = sizeof(randomValues);
    time_t t;

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
    if(ptr_randomValues == NULL) {
        perror("No mmap()");
        exit(0);
    }

    srand ((unsigned) time(&t));

    for(i = 0; i < ARR_SIZE; i++){

        ptr_randomValues -> arr[i] = 'A' + (random() % 26);
    }

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
/**
 * @author Ana Barreiro & Pedro Preto
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

#define SIZE 100

typedef struct {

    char randomValues[SIZE];
} st_randVal;

int main(){

    st_randVal *rv;
    int fd, i, st_size = sizeof(st_randVal), sum = 0;

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((fd = shm_open("/shmex04", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

        perror("Failed to create shared memory");
    }

    /*
     * ftruncate para ajustar o tamanho da struct à memoria partilhada
     */
    if(ftruncate(fd, st_size) < 0){

        perror("Failed to adjust memory size");
    }

    /*
    * mapear a memoria partilhada no nosso programa
    */
    rv = (st_randVal *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if (rv == NULL) {
        perror("No mmap()");
        exit(0);
    }

    /**
     * da print nos valores aleatórios
     */

    for (i = 0; i < SIZE; i++)
    {
        sum = sum + rv -> randomValues[i];

        printf("Char: %c\n", rv -> randomValues[i]);
    }

    /*
    * Desfaz o mapeamento
    */
    if(munmap(rv, st_size) < 0){

        perror("No munmap()");
        exit(0);
    }

    /*
     * Fecha o descritor
     */
    if(close(fd) < 0){

        perror("No close()");
        exit(1);
    }

    /*
     * Apaga a memoria partilhada do sistema
     */

    if(shm_unlink("/shmex04") < 0){

        perror("No unlink()");
        exit(1);
    }

    return 0;

}
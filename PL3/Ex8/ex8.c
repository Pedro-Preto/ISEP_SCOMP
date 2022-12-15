#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

/**
 * 8. Implement a program that creates a shared memory area to store an integer, 
 * initializes this value to 100, and creates a new process. 
 * Parent and child must perform the following operations 1.000.000 times:
 * • Increase the value
 * • Decrease the value
 * • Only write the value on the screen at the end
 * Review the results. Will these results always be correct?
 * 
 */

#define OPERATIONS 1000000
#define CHILDREN 1

/**
 * Função para criar processos filhos
 * 
 * @param n 
 * @return int 
 */
int babyMaker(int n){

    int p, i;
    for(i = 0; i < n; i++){

        p = fork();

        if(p < 0){

            exit(-1);
        }

        if(p == 0){

            /* filho */
            return i + 1;
        }
    }

    /* pai */
    return 0;
}

typedef struct{

    int value;

}ex8;

int main(){

    int shm, id, i;
    ex8 *ptr_ex8;
    int st_size = sizeof(ex8);

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((shm = shm_open("/shmex8", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

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
    ptr_ex8 = (ex8 *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if(ptr_ex8 == NULL) {
        perror("No mmap()");
        exit(0);
    }

    ptr_ex8 -> value = 100;

    id = babyMaker(CHILDREN);

    /* pai */
    if (id == 0){

        for(i = 0; i < OPERATIONS; i++){

            ptr_ex8 -> value++;
        }
        for(i = 0; i < OPERATIONS; i++){

            ptr_ex8 -> value--;
        }

        printf("From father: %d\n", ptr_ex8 -> value);
        wait(NULL);

        /* filho*/
    } else if (id > 0){

        for(i = 0; i < OPERATIONS; i++){

            ptr_ex8 -> value++;
        }
        for(i = 0; i < OPERATIONS; i++){

            ptr_ex8 -> value--;
        }

        printf("From son: %d\n", ptr_ex8 -> value);
        exit(0);
    }

    /*
     * Desfaz o mapeamento
     */
    if(munmap(ptr_ex8, st_size) < 0){

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

    /**
     * Apaga a memoria partilhada  
     */
    if (shm_unlink("/shmex8") < 0){

        perror("No unlink()");
        exit(1);
    }

    return 0;
}
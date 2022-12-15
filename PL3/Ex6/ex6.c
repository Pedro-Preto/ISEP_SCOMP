#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>


/**
 * @brief 
 * 
 * 6. Implement a solution that tests the speed of two transfer methods 
 * between two processes – pipes and shared memory. 
 * Start by filling a 1.000.000 array with numbers. 
 * Start a timer and copy that array between the two processes one element 
 * at a time using shared memory. Start a timer and use a pipe to transfer 
 * the same amount of data between processes. Test for different sizes of data
 * 
 */
#define ARR_SIZE 1000000
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

    int arr[ARR_SIZE];

    int flag;
}ex6;

int main(){

    int arr1[ARR_SIZE];
    int shm, id, i;
    time_t t;
    ex6 *ptr_ex6;
    int st_size = sizeof(ex6);
    clock_t start, end;

    /**
     * inicializa RNG (srand():stdlib.h; time(): time.h)
     */
    srand((unsigned)time(&t));

    /**
     * Preenche os arrays com números aleatórios
     */
    for(i = 0; i < ARR_SIZE; i++){

        arr1[i] = rand() % (ARR_SIZE + 1);
    }

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((shm = shm_open("/shmex6", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

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
    ptr_ex6 = (ex6 *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if(ptr_ex6 == NULL) {

        perror("No mmap()");
        exit(0);
    }

    ptr_ex6 -> flag = 0;

    id = babyMaker(CHILDREN);

    start = clock();

    /**
     * pai
     */
    if(id == 0){

        while(!ptr_ex6 -> flag);

        end = clock();
        printf("The transfer with shared memory took %f sec \n", ((double) (end - start)) / CLOCKS_PER_SEC);

    } else if(id > 0){

        for(i = 0; i < ARR_SIZE; i++){

            ptr_ex6 -> arr[i] = arr1[i];
            ptr_ex6 -> flag = 1;
        }

        ptr_ex6 -> flag = 1;

        exit(0);

    }

    /*
     * Desfaz o mapeamento
     */
    if(munmap(ptr_ex6, st_size) < 0){

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
    if (shm_unlink("/shmex6") < 0){
        perror("No unlink()");
        exit(1);
    }

    /**
     * Com Pipes
     */
    ex6 *ptr_ex6_pipe;
    int fd[2];

    if(pipe(fd) < 0){

        perror("Pipe failed!\n");
        return 1;
    }

    id = babyMaker(CHILDREN);

    start = clock();

    /* pai */
    if(id == 0){

        /* fecha a extremidade de escrita */
        close(fd[1]);

        /* lê do pipe */
        for(i = 0; i < ARR_SIZE; i++){

            read(fd[0], &ptr_ex6_pipe -> arr[i], st_size);
        }

        /* fecha a extremidade de leitura */
        close(fd[0]);

        end = clock();

        printf("The transfer with a pipe took %f sec \n", ((double) (end - start)) / CLOCKS_PER_SEC);

    } else if (id > 0){

        /* fecha a extremidade de leitura */
        close(fd[0]);

        for(i = 0; i < ARR_SIZE; i++){

            write(fd[1], &arr1[i], st_size);
        }

        /* fecha a extremidade de escrita */
        close(fd[1]);
        exit(0);
    }

    return 0;


}
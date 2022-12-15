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
 * 12. Implement a program that allows the exchange of data concerning 
 * a student between two processes (number, name and grades of a set of classes). 
 * The data to be exchanged are represented in the following struct aluno.
 */


#define STR_SIZE 50
#define NR_DISC 10
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

struct aluno{

    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
};

int main(){

    int shm, id, i;
    struct aluno *ptr_aluno;
    int st_size = sizeof(struct aluno);

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((shm = shm_open("/shmex12", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

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
    ptr_aluno = (struct aluno *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if(ptr_aluno == NULL) {

        perror("No mmap()");
        exit(0);
    }

    printf("Insert student's number:\n");
    scanf("%d", &(ptr_aluno -> numero));
    fflush(stdin);
    getchar();

    printf("Insert student's name:\n");
    scanf("%50[^\n]", ptr_aluno -> nome);
    fflush(stdin);

    for(i = 1; i <= NR_DISC; i++){

        printf("Insert student's Grade on Subject %d: \n", i);
        scanf("%d", &(ptr_aluno -> disciplinas[i - 1]));
        fflush(stdin);
        getchar();
    }

    id = babyMaker(CHILDREN);


    /* pai */
    if(id == 0){

        wait(NULL);

        /* filho */
    } else if(id > 0){

        int i, lowest, highest, average, sum;

        lowest = ptr_aluno -> disciplinas[0];
        highest = ptr_aluno -> disciplinas[0];
        average = ptr_aluno -> disciplinas[0];

        for(i = 0; i < NR_DISC; i++){

            if (ptr_aluno -> disciplinas[i] < lowest){

                lowest = ptr_aluno -> disciplinas[i];
            }
            if (ptr_aluno -> disciplinas[i] > highest){

                highest = ptr_aluno -> disciplinas[i];
            }
            sum = sum + ptr_aluno -> disciplinas[i];
        }

        average = sum / NR_DISC;

        printf("--> Student Statistics <--\n");

        printf("Student's Number: %d\n", ptr_aluno -> numero);
        printf("Student's Name: %s\n", ptr_aluno -> nome);
        printf("Lowest grade = %d\n", lowest);
        printf("Highest grade = %d\n", highest);
        printf("Average from all grades = %d\n", average);

    }

    /*
     * Desfaz o mapeamento
     */
    if(munmap(ptr_aluno, st_size) < 0){

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
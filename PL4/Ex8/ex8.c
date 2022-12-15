#include <errno.h>
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
#include <semaphore.h>

/**
 * 8. Write a program that creates a new process. 
 * The father and the child should indefinitely write the letters ’S’ and ’C’
 * Implement a policy based on semaphores such that at any moment the number 
 * of ’S’ or ’C’ differs by at most one. The solution should allow strings such as: 
 * “SCCSSCCSCS”
 */

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

/**
 * If the value is 1
 * wait() - the value drops from 1 to 0
 * 
 * If the value is 0 
 * wait() - keeps waiting :))
 */
void down(sem_t *sem){

    if(sem_wait(sem) == -1){

        perror("Error at sem_wait()!\n");
    }
}

/**
 * post() - the value increases
 */
void up(sem_t *sem){

    if(sem_post(sem) == -1){

        perror("Error at sem_post()!\n");
    }
}

int main(){

    sem_t *sem1;
    sem_t *sem2;
    int i = 0;

    /**
     * Cria e abre o semáforo 1 com permissões 0644 
     * (owner: read and write; group: read; other: read)
     */
    if((sem1 = sem_open("sem_ex8_1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){

        perror("Error at sem_open()!\n");
        exit(0);
    }

    /**
     * Cria e abre o semáforo 2 com permissões 0644 
     * (owner: read and write; group: read; other: read)
     */
    if((sem2 = sem_open("sem_ex8_2", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){

        perror("Error at sem_open()!\n");
        exit(0);
    }

    /**
     * Criar um novo processo
     */
    int id = babyMaker(CHILDREN);

    /* pai */
    if(id == 0){

        while ( i == 0){

            sem_wait(sem1);
            printf("S");
            fflush(stdout);
            sem_post(sem2);
        }

    } else if (id > 0){

        while ( i == 0){

            sem_wait(sem2);
            printf("C");
            fflush(stdout);
            sem_post(sem1);
        }

    }

    /**
     * Fecha o semáforo 1
     */
    if(sem_close(sem1) == -1){

        perror("Error at sem_close()!\n");
        exit(0);
    }

    /**
     * Apaga o semáforo 1
     */
    if(sem_unlink("sem_ex8_1") == -1){

        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }

    /**
     * Fecha o semáforo 2
     */
    if(sem_close(sem2) == -1){

        perror("Error at sem_close()!\n");
        exit(0);
    }

    /**
     * Apaga o semáforo 2
     */
    if(sem_unlink("sem_ex8_2") == -1){

        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }

    return 0;
}
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/**
 * 15. Consider a show room facility and a set of visitors, who randomly enter a room. 
 * Each visitor is simulated by an infinite loop in which he enters a room and leaves 
 * at the end of the show. Each show in a room starts and ends at defined time instants; 
 * the visitors can only enter and leave the room at those strict times. 
 * Each room supports 5 visitors; once the room is full, further visitors have to wait. 
 * For simulation purposes consider that: 
 * 
 * (a) you have 25 visitors, 4 exhibition rooms and one lobby; 
 * (b) the duration of each show is 5 seconds; 
 * (c) all shows start and end at the same time, with 1 second delay between the end of 
 * a show and the start of the next; 
 * (d) visitors wait in a lobby until they are allowed to enter in a random room.
 * 
 * Print the necessary message to clearly show the status of the show room. 
 * Visitors and rooms are represented by different processes.
 */

#define MAX_VISITORS 5
#define VISITORS 25
#define ROOMS 4
#define DURATION 5
#define DELAY 1

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

/**
 * para controlar os lugares da sala
 */
struct room{

    int num;
};

int main(){

    int i, j, id, shm;

    struct room *ptr_room;
    int st_size = sizeof(struct room);

    /**
     * semaforo 1 para controlar as entradas na sala 
     * semaforo 2 para controlar o inicio do show
     * semadoro 3 para controlar as saidas da sala
     */
    sem_t *sem_in_1;
    sem_t *sem_show_2;
    sem_t *sem_out_3;

    /**
     * Cria e abre o semáforo 1 com permissões 0644 
     * (owner: read and write; group: read; other: read)
     * semaforo 1 para controlar as entradas na sala 
     */
    if((sem_in_1 = sem_open("sem_ex15_1", O_CREAT | O_EXCL, 0644, MAX_VISITORS)) == SEM_FAILED){

        perror("Error at sem_open()!\n");
        exit(0);
    }

    /**
     * Cria e abre o semáforo 2 com permissões 0644 
     * (owner: read and write; group: read; other: read)
     * semaforo 2 para controlar o inicio do show
     */
    if((sem_show_2 = sem_open("sem_ex15_2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED){

        perror("Error at sem_open()!\n");
        exit(0);
    }

    /**
     * Cria e abre o semáforo 1 com permissões 0644 
     * (owner: read and write; group: read; other: read)
     * semadoro 3 para controlar as saidas da sala
     */
    if((sem_out_3 = sem_open("sem_ex15_3", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){

        perror("Error at sem_open()!\n");
        exit(0);
    }

    /*
     Cria e verifica se a memória partilhada foi devidamente aberta
     permissões de read, write, execute/search para user, grupo e owner
    */
    if((shm = shm_open("/shmepl4ex15", O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)) < 0){

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
    ptr_room = (struct room *)mmap(NULL, st_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

    /*
     * Verifica se a memória partilhada foi devidamente mapeada
     */
    if(ptr_room == NULL) {

        perror("No mmap()");
        exit(0);
    }

    for(j = 0; j < VISITORS; j++){

        id = babyMaker(VISITORS);
        /* pai */
        if(id == 0){

            wait(NULL);

        } else if (id > 0) {

            while (1){

                down(sem_in_1);
                printf("Visitor %d entered the room\n", id);

                ptr_room -> num++;

                /* verificar se o show pode começar */
                if(ptr_room -> num == MAX_VISITORS){

                    for(i = 0; i < MAX_VISITORS; i++){

                        up(sem_show_2);
                    }
                    printf("Show started!\n");
                }

                down(sem_show_2);

                sleep(DURATION);

                /* fim do show */

                down(sem_out_3);
                ptr_room -> num--;
                up(sem_out_3);

                if(ptr_room -> num == 0){

                    printf("Next show in %d second!\n", DELAY);
                    sleep(DELAY);

                    for(i = 0; i > MAX_VISITORS; i++){
                        up(sem_in_1);
                    }
                }

                exit(0);

            }
        }
    }

    /**
     * Fecha o semáforo 1
     */
    if(sem_close(sem_in_1) == -1){

        perror("Error at sem_close()!\n");
        exit(0);
    }

    /**
     * Apaga o semáforo 1
     */
    if(sem_unlink("sem_ex15_1") == -1){

        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }

    /**
     * Fecha o semáforo 2
     */
    if(sem_close(sem_show_2) == -1){

        perror("Error at sem_close()!\n");
        exit(0);
    }

    /**
     * Apaga o semáforo 2
     */
    if(sem_unlink("sem_ex15_2") == -1){

        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }

    /**
     * Fecha o semáforo 3
     */
    if(sem_close(sem_out_3) == -1){

        perror("Error at sem_close()!\n");
        exit(0);
    }

    /**
     * Apaga o semáforo 3
     */
    if(sem_unlink("sem_ex15_3") == -1){

        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(0);
    }

    /*
     * Desfaz o mapeamento
     */
    if(munmap(ptr_room, st_size) < 0){

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
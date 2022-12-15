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
#include <semaphore.h>

#define MAX_EXEC 15

typedef struct {
    int counter;
} sh_data;

int main() {
    sem_t *sem1, *sem2;
	// Criar semáforos
    if ((sem1 = sem_open("/sem1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((sem2 = sem_open("/sem2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    int fd, r, data_size = sizeof(sh_data);
    sh_data *data;
    // Criar shared memory
    if ((fd = shm_open("/shmscomp", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0)
        return 1; // Erro ao criar memória partilhada
    if (ftruncate(fd, data_size) < 0)
        return 1; // Erro a definir tamanho do espaço de memória
    if ((data = (sh_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
        return 1; // Erro a mapear memória

    data->counter = 0;

    int c;
    pid_t p = fork();
    if (p < 0) return 1; // Erro no fork
    for (;;) {
        if (p > 0) {
            sem_wait(sem2);
            printf("I'm the father\n");
            c = data->counter++;
            sem_post(sem1);
            if (c >= MAX_EXEC)
                break;
        } else {
            sem_wait(sem1);
            printf("I'm the child\n");
            c = data->counter++;
            sem_post(sem2);
            if (c >= MAX_EXEC)
                break;
        }
    }

    if (p > 0) {
        wait(NULL);
        
        // Remover memória partilhada
        if ((r = munmap(data, data_size)) < 0) exit(1);
        if ((r = shm_unlink("/shmscomp")) < 0) exit(1); // Erro ao eliminar memória partilhada
    } else {
        // Fechar ligação à memória partilhada
        if ((r = munmap(data, data_size)) < 0) exit(1);
        if ((r = close(fd)) < 0) exit(1); // Erro ao fechar descritor

        exit(0);
    }

    if (sem_unlink("/sem1") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
    if (sem_unlink("/sem2") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
}

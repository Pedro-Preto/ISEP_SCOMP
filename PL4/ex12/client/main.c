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

typedef struct {
    int ticket;
} sh_data;

int main() {
	// Criar semáforos
    sem_t *sv, *cli, *atendimento;
    if ((sv = sem_open("/semsv", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((cli = sem_open("/semcli", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((atendimento = sem_open("/semclival", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    int fd, r, data_size = sizeof(sh_data);
    sh_data *data;
    // Criar shared memory
    if ((fd = shm_open("/shmex12", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0)
        return 1; // Erro ao criar memória partilhada
    if (ftruncate(fd, data_size) < 0)
        return 1; // Erro a definir tamanho do espaço de memória
    if ((data = (sh_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
        return 1; // Erro a mapear memória

    sem_wait(cli);
    sem_post(sv);
    sem_wait(atendimento);
    int ticket = data->ticket;
    printf("Ticket number %d!\n", ticket);
    sem_post(cli);
    
    // Fechar ligação à memória partilhada
    if ((r = munmap(data, data_size)) < 0) exit(1);
    if ((r = close(fd)) < 0) exit(1); // Erro ao fechar descritor
}

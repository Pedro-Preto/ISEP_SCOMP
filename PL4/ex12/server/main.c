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

void handle_SIGINT(int signo, siginfo_t *sinfo, void *context) {
    if (sem_unlink("/semsv") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
    if (sem_unlink("/semcli") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
    if (sem_unlink("/semclival") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
    exit(0);
}

int main() {
    sem_t *sv, *cli, *clival;
    if ((sv = sem_open("/semsv", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((cli = sem_open("/semcli", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((clival = sem_open("/semclival", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    // Handle CTRL+C
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_SIGINT;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act, NULL);

    int fd, data_size = sizeof(sh_data);
    sh_data *data;
    // Criar shared memory
    if ((fd = shm_open("/shmex12", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0)
        return 1; // Erro ao criar memória partilhada
    if (ftruncate(fd, data_size) < 0)
        return 1; // Erro a definir tamanho do espaço de memória
    if ((data = (sh_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
        return 1; // Erro a mapear memória

    srand((unsigned) time(NULL));

    printf("CTRL+C to end server execution\n");

    int next = 1;
    for (;;) {
        sem_wait(sv);
        data->ticket = next++;
        sleep(rand() % 10 + 1);
        sem_post(clival);
    }
    
}

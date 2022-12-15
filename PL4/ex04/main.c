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

#define ARR_SIZE 50
#define STR_SIZE 80

typedef struct {
    char elem[ARR_SIZE][STR_SIZE];
} sh_data;

int main() {
    sem_t *sem;
    if ((sem = sem_open("/semex4", O_CREAT, 0644, 1)) == SEM_FAILED) {
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
	
	//Random Number Generator
    srand((unsigned) time(NULL));

    int opt, pos;
    printf("=== Menu ===\n1. Add\n2. Remove\n\nSelect option: ");
    scanf("%d%*c", &opt);
    if (opt != 1 && opt != 2) {
        perror("Invalid option");
        exit(1);
    }

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("Error defining timespec");
        return -1;
    }
	
    ts.tv_sec += 12; //conversão para segundos
    if (sem_timedwait(sem, &ts) < 0) {
        perror("12 seconds timeout");
        exit(1);
    }

    if (opt == 1) {
        printf("Position [0-49]: ");
        scanf("%d%*c", &pos);
        if (pos < 0 || pos > 49) {
            perror("Invalid position");
            exit(1);
        }

        char str[STR_SIZE];
        sprintf(str, "I'm the Father - with PID %d", getpid());
        strcpy(data->elem[pos], str);

        int wait = rand() % 5 + 1;
        sleep(wait);

        printf("%s\n", data->elem[pos]);
    } else {
        printf("Position [0-49]: ");
        scanf("%d%*c", &pos);
        if (pos < 0 || pos > 49) {
            perror("Invalid position");
            exit(1);
        }

        *(data->elem[pos]) = 0;
    }

    sem_post(sem);

    // Fechar ligação à memória partilhada
    if ((r = munmap(data, data_size)) < 0) exit(1);
    if ((r = close(fd)) < 0) exit(1); // Erro ao fechar descritor

    if (sem_unlink("/semex4") < 0) {
        perror("Error in sem_unlink()");
        exit(1);
    }
	return 0;
}

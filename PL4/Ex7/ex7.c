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


#define SEMAFORO_01 0
#define SEMAFORO_02 1
#define SEMAFORO_03 2

int cria_filhos(int n) {
	pid_t pid;
	int i;
	for (i = 0; i < n; i++) {
		pid = fork();
		if (pid < 0)
			return -1;
		else if (pid == 0)
			return i + 1;
	}
	return 0;
}

int main(){
    const char SEMAFOROS_NAME[3][80] = {"SEM_01", "SEM_02", "SEM_03"};
    const int SEMAFOROS_INITIAL_VALUE[3] = {1, 0, 0};

    sem_t *semaforos[3] ;

      // Create semaphore
    for(int i=0;i<3;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
int id=cria_filhos(3);
 
        if (id == 1) {
		sem_wait(semaforos[SEMAFORO_01]);
		printf("Sistemas \n");
		sem_post(semaforos[SEMAFORO_02]);

		sem_wait(semaforos[SEMAFORO_01]);
		printf("a \n");
		sem_post(semaforos[SEMAFORO_02]);
		
		exit(1);
	}
	if (id == 2) {
		sem_wait(semaforos[SEMAFORO_02]);
		printf("de \n");
		sem_post(semaforos[SEMAFORO_03]);

		sem_wait(semaforos[SEMAFORO_02]);
		printf("melhor \n");
		sem_post(semaforos[SEMAFORO_03]);
		
		exit(2);
	}

	if (id == 3) {
		sem_wait(semaforos[SEMAFORO_03]);
		printf("Computadores - \n");
		sem_post(semaforos[SEMAFORO_01]);

		sem_wait(semaforos[SEMAFORO_03]);
		printf("disciplina! \n");
		
		exit(3);
	}

	//Pai eseprando todos os filhos
	for (int i = 0; i < 3; i++) {
		wait(NULL);
	}

	//Cicl para fechar todos os semaforos
	for (int i = 0; i < 3; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Cicl para remoer todos os semaforos do SO
	for (int i = 0; i < 3; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}

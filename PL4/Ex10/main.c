#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <time.h> 
#define MY_SHARED_FILE "/ex10shm"
#define SEMAFORO "/ex10shm"

int menu() ;
void runProgram(char *program) ;


typedef struct{
    char name[10];
    int number;
    char address[20];
} users_info;

typedef struct {
    users_info logins[100];
    int index;
}loginsQtd;
#define STRUCT_SIZE sizeof(loginsQtd)


int main(){
    int fd;
    sem_t *semaphore;

    loginsQtd *loginStruct;

    shm_unlink(MY_SHARED_FILE);
    sem_unlink(SEMAFORO);

    fd = shm_open(MY_SHARED_FILE, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){
        printf("Error while creating shared memory\n");
        exit(EXIT_FAILURE);
    }

   ftruncate(fd, STRUCT_SIZE);
    loginStruct = (loginsQtd*)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
       // printf("Index:%d\n",loginStruct->index);


    if((semaphore = sem_open(SEMAFORO, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){ //semaforo exclusao mutua um de cada vez
        semaphore = sem_open(SEMAFORO, O_CREAT);
    }
    loginStruct->index = 0;//primeira

  
    int option;

    do {
        option = menu();

        switch (option) {
            case 1:
                runProgram("./insert.bin");
                break;
            case 2:
                runProgram("./consult.bin");
                break;
            case 3:
                runProgram("./consult_all.bin");
                break;
            case 0:
                return EXIT_SUCCESS;
            default:
                option = menu();
        }
    } while(option != 0);

   if (munmap(loginStruct, STRUCT_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
   if (shm_unlink(MY_SHARED_FILE) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
	}	

	/* fechar os semáforos */
		if (sem_close(semaphore) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		
	}

	//Cicl para remover todos os semaforos do SO
		if (sem_unlink(SEMAFORO) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	

    return EXIT_SUCCESS;
}


int menu() {
    int option;
    printf("1. Insert\n");
    printf("2. Consult One Record\n");
    printf("3. Consult All Records\n");
    printf("0. Exit\n");
    scanf("%d", &option);
    return option;
}

void runProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_FAILURE);
    }
    while (wait(NULL) > 0) {
        // wait until all children are dead
    };
}
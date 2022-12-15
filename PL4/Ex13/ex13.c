#include<stdio.h>
#include<time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>

#define SEMAFORO_WRITER 0
#define SEMAFORO_READER 1
#define QUANTIDADE_SEMAFOROS 2
#define SIZE 10
#define OPERACOES 30

typedef struct {
    int numbers[SIZE];
} shared_data_type;


int main(){

	const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80] = {"WRITTER", "READER"};
    const int SEMAFOROS_INITIAL_VALUE[QUANTIDADE_SEMAFOROS] = {1,0};

   sem_t *semaforos[QUANTIDADE_SEMAFOROS] ;

      // Create semaphore
    for(int i=0;i<QUANTIDADE_SEMAFOROS;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
	
	shared_data_type *shared_data;
  	
	//size of data
    int size = sizeof(shared_data);
    
	// Creates and opens a shared memory area
    int fd = shm_open("/ex13", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
	//====================================================================================================================================
	  int i=0,sequence=0,j=0;


    for(i = 0; i < 2; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) {

            for(i=0; i<(OPERACOES/SIZE); i++){//Para escrever os 30 valores ou seja vai executar 3 ciclos em cada um deles vai escrever 10 valores
                
                for(j = 0;j<SIZE;j++ ){
                    sequence++;
                    sem_wait(semaforos[SEMAFORO_WRITER]);
                    shared_data->numbers[j]=sequence;
                    if(j+1!=SIZE){//se o array estiver totalmente preenchido paro de escrever
                        sem_post(semaforos[SEMAFORO_WRITER]);
                    }
                }        
					//esta totalmente preenchido por isso o pai pode escrever
                    sem_post(semaforos[SEMAFORO_READER]);

            }
            
            exit(0);

    }else{
        for(i=0; i<(OPERACOES/SIZE); i++){
            //espera por autorização
            sem_wait(semaforos[SEMAFORO_READER]);
           
            for(j = 0;j<SIZE;j++ ){                
                printf("Value of index %d is %d\n",j,shared_data->numbers[j]);
            }
			//Quando acaba de escrever dá autorização ao writer
            sem_post(semaforos[SEMAFORO_WRITER]);

        }
        }
    }


	//====================================================================================================================================
	
	// Undo mapping
    if (munmap(shared_data, size) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
   if (shm_unlink("/ex13") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
	}	


	/* fechar os semáforos */
	for (int i = 0; i < QUANTIDADE_SEMAFOROS; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Cicl para remover todos os semaforos do SO
	for (int i = 0; i < QUANTIDADE_SEMAFOROS; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}

    return 0;

}

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
#include <errno.h>
#include <unistd.h>

#define SHM_NAME "/shmEX"
#define CHILD_NUMBER 5

#define SEMAFORO_02 1
#define SEMAFORO_03 2
#define SEMAFORO_04 3
#define SEMAFORO_05 4


int spawnChilds(int n);


typedef struct
{
    char name[5][4];
    int time[5];
    
} shared_data_type;
	

int main() {
	 	

	const char SEMAFOROS_NAME[CHILD_NUMBER][80] = {"SEM_01", "SEM_02", "SEM_03", "SEM_04", "SEM_05"};
    const int SEMAFOROS_INITIAL_VALUE[CHILD_NUMBER] = {0, 0, 0, 0 ,0};

    sem_t *semaforos[CHILD_NUMBER] ;

      // Create semaphore
    for(int i=0;i<CHILD_NUMBER;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
	
	shared_data_type *shared_data;

	//shared memory size 
	int size = sizeof(shared_data_type);
	
	// Creates and opens a shared memory area
	int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

	ftruncate(fd, size);

	// Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	
	//Create all 5 Child Processes	
	int id = spawnChilds(5);
	
	
		if(id==1){
		
 
		time_t t; /* to init. the random number generator (RNG)*/

		srand ((unsigned) time (&t));
		int time = rand () % 10+1; /* inteiro de 1 a 10 */
		
		sleep(time);  //Wait
		
		char buffer[4];
		
		snprintf(buffer, sizeof(buffer), "A%d", id); // Create name for the algorithme with is id
		


		strcpy(shared_data->name[id-1], buffer); // Save name 
		shared_data->time[id-1] = time; //save time
		
	
		printf("Algoritmo 1 Terminado - os dados foram inseridos na região de memória partilhada");
		
		sem_post(semaforos[SEMAFORO_02]); 
		
		exit(0);
		}

		if(id==2){
		
		sem_wait(semaforos[SEMAFORO_02]); 
 
		time_t t; /* to init. the random number generator (RNG)*/

		srand ((unsigned) time (&t));
		int time = rand () % 10+1; /* inteiro de 1 a 10 */
		
		sleep(time);  //Wait
		
		char buffer[4];
		
		snprintf(buffer, sizeof(buffer), "A%d", id); // Create name for the algorithme with is id
		

		strcpy(shared_data->name[id-1], buffer); // Save name 
		shared_data->time[id-1] = time; //save time
		
	
		printf("Algoritmo 2 Terminado - os dados foram inseridos na região de memória partilhada");
		
		sem_post(semaforos[SEMAFORO_03]); 
		
		exit(0);
		}

		if(id==3){
		sem_wait(semaforos[SEMAFORO_03]); 

 
		time_t t; /* to init. the random number generator (RNG)*/

		srand ((unsigned) time (&t));
		int time = rand () % 10+1; /* inteiro de 1 a 10 */
		
		sleep(time);  //Wait
		
		char buffer[4];
		
		snprintf(buffer, sizeof(buffer), "A%d", id); // Create name for the algorithme with is id
		

		strcpy(shared_data->name[id-1], buffer); // Save name 
		shared_data->time[id-1] = time; //save time
		
	
		printf("Algoritmo 3 Terminado - os dados foram inseridos na região de memória partilhada");
		
		sem_post(semaforos[SEMAFORO_04]); 
		
		exit(0);
		}

		if(id==4){
		sem_wait(semaforos[SEMAFORO_04]); 

 
		time_t t; /* to init. the random number generator (RNG)*/

		srand ((unsigned) time (&t));
		int time = rand () % 10+1; /* inteiro de 1 a 10 */
		
		sleep(time);  //Wait
		
		char buffer[4];
		
		snprintf(buffer, sizeof(buffer), "A%d", id); // Create name for the algorithme with is id
		


		strcpy(shared_data->name[id-1], buffer); // Save name 
		shared_data->time[id-1] = time; //save time
		
	
		printf("Algoritmo 4 Terminado - os dados foram inseridos na região de memória partilhada");
		
		sem_post(semaforos[SEMAFORO_05]); 
		
		exit(0);
		}

		if(id==5){
		sem_wait(semaforos[SEMAFORO_05]); 

 
		time_t t; /* to init. the random number generator (RNG)*/

		srand ((unsigned) time (&t));
		int time = rand () % 10+1; /* inteiro de 1 a 10 */
		
		sleep(time);  //Wait
		
		char buffer[4];
		
		snprintf(buffer, sizeof(buffer), "A%d", id); // Create name for the algorithme with is id
		

		strcpy(shared_data->name[id-1], buffer); // Save name 
		shared_data->time[id-1] = time; //save time
		
	
		printf("Algoritmo 5 Terminado - os dados foram inseridos na região de memória partilhada");
		
		
		exit(0);
		}
	
	    
    for (int i = 0; i < CHILD_NUMBER; i++) {  // wait for childs
		wait(NULL);
	}


	printf("============MELHOR TEMPO=============\n");
	int melhorTempo=-1;
	for(int i=0;i<CHILD_NUMBER-1;i++){
		if(shared_data->time[i]>=shared_data->time[i+1]){
		melhorTempo=shared_data->time[i+1];
		}else{
		melhorTempo=shared_data->time[i];
		}

	}
	printf("Processo %s com tempo %d\n",shared_data->name[melhorTempo],shared_data->time[melhorTempo]);
	printf("======================================\n");

	//undo mapping 		
	if(munmap(shared_data, size) == -1){
		perror("Failed munmap\n");
		exit(EXIT_FAILURE);
	}
	//close descriptor
	if(close(fd) == -1){
		perror("Failed close\n");
		exit(EXIT_FAILURE);
	}

	   // Remove file from system
    if (shm_unlink(SHM_NAME) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
	}	
	//Cicl para fechar todos os semaforos
	for (int i = 0; i < CHILD_NUMBER; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Cicl para remover todos os semaforos do SO
	for (int i = 0; i < CHILD_NUMBER; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}

	 
	return 0;

}


int spawnChilds(int n){
	
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

	


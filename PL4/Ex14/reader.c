//Writes its PID and current time.
//Writers have priority over readers.
//Only one writer can access the shared memory area at the same time
//Each writer prints the number of writers and also the number of readers

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

#define MY_SHARED_FILE "/Ex_14"
#define SEMAFORO_1 0
#define SEMAFORO_2 1
#define SEMAFORO_3 2
#define QUANTIDADE_SEMAFOROS 3
#define QUANTIDADE_READERS 4
#define QUANTIDADE_WRITERS 7


typedef struct{
    char string[100];
    int currentReaders;
    int nReaders;
    int nWriters;
} shared_memory_area;

#define STRUCT_SIZE sizeof(shared_memory_area)

void sharedMemoryClosure(int fd,shared_memory_area *shared_data);



int main(){
    
    //|=========================|
    //criacao dos semaforos
	const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80] = {"ACESSO_MEMORIA", "ESCRITA","PRIORIDADE"};   //nome dos semaforos
    const int SEMAFOROS_INITIAL_VALUE[QUANTIDADE_SEMAFOROS] = {1,1,1};               //valores iniciais de cada semaforo   

   sem_t *semaforos[QUANTIDADE_SEMAFOROS] ;                                         //array de semaforos

      // Create semaphore
    for(int i=0;i<QUANTIDADE_SEMAFOROS;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()-Reader");
            exit(1);
        }
    }
    //==============================|
	//Criacao da memoria partilhada  	
    
	// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, STRUCT_SIZE);

    // Get a pointer to the data
    shared_memory_area *shared_data = (shared_memory_area *)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //============================================================================================================================================================
	//SEMAFORO PARA GARANTIR QUE WRITTER TEM PRIORIDADE
    sem_wait(semaforos[SEMAFORO_3]);
    sem_post(semaforos[SEMAFORO_3]);
    
    
    //SEMAFORO PARA MEXER NA MEMORIA POR ISSO MAIS NINGUEM PODE ESTAR A MEXER
    sem_wait(semaforos[SEMAFORO_1]);

    
    //AUMENTAR NUMERO DE READERS CURRENTES E DE TOTAIS
    shared_data->currentReaders++;
    shared_data->nReaders++;

    
    //SE FORMOS O UNICO READER VAMOS ATIVAR O SEMAFORO DE NO USE PARA NINGUEM PODER ESCREVER AO MESMO TEMPO
    if( shared_data->currentReaders==1){
        sem_wait(semaforos[SEMAFORO_2]);
    }
    
    
    //LIBERTAR SEMAFORO POIS NAO ESTAMOS A ESCREVER
    sem_post(semaforos[SEMAFORO_1]);
    
    //imprimir string
    printf("%s", shared_data->string);
    
    
    //VAMOS ESCREVER NOVAMENTE POR ISSO VAMOS BLOQUEAR SEMAFORO DE EXCLUSAO
    sem_wait(semaforos[SEMAFORO_1]);
     shared_data->currentReaders--;

    
    //SE NAO HOUVER LEITORES DAMOS POST NO SEMAFORO DE ESCRITA PARA QUE POSSO HAVER ESCRITA
    if( shared_data->currentReaders==0){
        sem_post(semaforos[SEMAFORO_2]);
    }
    
    
    //LIBERTAR SEMAFORO DE EXCLUSAO
    sem_post(semaforos[SEMAFORO_1]);
    
    
    //===================================================CLOSE SHARED MEMORY AND SEMAPHORES========================================================================
	
    sharedMemoryClosure(fd,shared_data);


    return 0;

}

void sharedMemoryClosure(int fd,shared_memory_area *shared_data){

// Undo mapping
    if (munmap(shared_data, STRUCT_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
}


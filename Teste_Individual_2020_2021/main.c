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
#include <sys/wait.h>

#define MY_SHARED_FILE "/Ex_Individual"
#define SEMAFORO_WRITER_1 0
#define SEMAFORO_WRITER_2 1
#define SEMAFORO_READER 2
#define QUANTIDADE_SEMAFOROS 3
#define QUANTIDADE_FILHOS 3
#define SIZE_ARRAY_DEZ_MIL 10000
#define SIZE_ARRAY_CINCO_MIL 5000

int spawnChilds(int n);
int maxChecker(int max,int test);

typedef struct {
    int numbers[SIZE_ARRAY_CINCO_MIL];
    int max;
    int posicao;
    int flag;

} shared_data_type;
#define STRUCT_SIZE sizeof(shared_data_type)


int main(){

	const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80] = {"WRITTER_1", "WRITTER_2","READER"};
    const int SEMAFOROS_INITIAL_VALUE[QUANTIDADE_SEMAFOROS] = {1,0,0};

   sem_t *semaforos[QUANTIDADE_SEMAFOROS] ;

      // Create semaphore
    for(int i=0;i<QUANTIDADE_SEMAFOROS;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
	  	
    
	// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, STRUCT_SIZE);

    // Get a pointer to the data
    shared_data_type *shared_data = (shared_data_type *)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
	//====================================================================================================================================
	
     int totalNum[SIZE_ARRAY_DEZ_MIL];
    srand((unsigned) time(NULL) * getpid()); 

    //carregar os dados para o array de 10000 integers
    for(int i=0;i<SIZE_ARRAY_DEZ_MIL;i++){
        totalNum[i]=(rand() % 50)+1;//Valores entre 1 e 50
    }
    
    
    //Inicializar a flag
    shared_data->max=0;
    shared_data->posicao=0;
    shared_data->flag=0;

    //criar 3 filhos
    int id=spawnChilds(QUANTIDADE_FILHOS);
    
            if(id==1){//=======P1
                for(int i=0;i<SIZE_ARRAY_DEZ_MIL-1;i+=4){

                sem_wait(semaforos[SEMAFORO_WRITER_1]);//espera autorizaçao

                shared_data->numbers[shared_data->posicao]=totalNum[i]*totalNum[i+1];

               // printf("P1-Posição:%d e Posição:%d com valor1:%d e valor2:%d no ARRAY[%d]=%d \n",i,i+1,totalNum[i],totalNum[i+1],shared_data->posicao,totalNum[i]*totalNum[i+1]);

                    
                    if(maxChecker(shared_data->max,shared_data->numbers[shared_data->posicao])==1){//verificar se a multiplicacao é maior q o max
                        shared_data->max=shared_data->numbers[shared_data->posicao];
                        sem_post(semaforos[SEMAFORO_READER]);//P3 pode escrever
                    }       

                shared_data->posicao++;
                sem_post(semaforos[SEMAFORO_WRITER_2]);//P2 pode escrever
                }
            
            printf("P1 FINALIZOU\n");
            exit(EXIT_SUCCESS);
            }

            if(id==2){//======P2
                for(int i=2;i<SIZE_ARRAY_DEZ_MIL;i+=4){
                    sem_wait(semaforos[SEMAFORO_WRITER_2]);//espera autorizaçao
                
                    shared_data->numbers[shared_data->posicao]=totalNum[i]*totalNum[i+1];

                   // printf("P2-Posição:%d e Posição:%d com valor1:%d e valor2:%d no ARRAY[%d]=%d \n",i,i+1,totalNum[i],totalNum[i+1],shared_data->posicao,totalNum[i]*totalNum[i+1]);

                    
                    if(maxChecker(shared_data->max,shared_data->numbers[shared_data->posicao])==1){//verificar se a multiplicacao é maior q o max
                        shared_data->max=shared_data->numbers[shared_data->posicao];
                        sem_post(semaforos[SEMAFORO_READER]);//P3 pode escrever
                    }       

                shared_data->posicao++;
                sem_post(semaforos[SEMAFORO_WRITER_1]);//P1 pode escrever
                }

            shared_data->flag=1;
            printf("P2 FINALIZOU\n");
            sem_post(semaforos[SEMAFORO_READER]);//P3 pode escrever-Neste Ponto dá autorizacao mas como a flag é 1 o processo termina
            exit(EXIT_SUCCESS);
            }
        
    
    if(id==3){//P3
        while( shared_data->flag==0)
        {
            sem_wait(semaforos[SEMAFORO_READER]);//espera autorizaçao
            if(shared_data->flag==0){

                printf("======================================\n");

                printf("Foi encontrado um novo Produto Max:%d\n",shared_data->max);
                        
                printf("======================================\n");
            }
        }
        printf("P3 FINALIZOU\n");
    exit(EXIT_SUCCESS);
    }
  

    //===========================================================PAI

    for(int f=0;f<3;f++){
		wait(NULL);
    }
    printf("===================================\n");
    printf("===================================\n");
    printf("Produto Max:%d\n",shared_data->max);
     printf("===================================\n");
    printf("===================================\n");
    for(int j=0;j<SIZE_ARRAY_CINCO_MIL;j++){
        printf("Array[%d]:%d\n",j,shared_data->numbers[j]);

    }

    printf("===================================\n");
    printf("===================================\n");

	//====================================================================================================================================
	
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
   if (shm_unlink(MY_SHARED_FILE) < 0) {
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
int maxChecker(int max,int test){
    if(max>=test){
        return 0;
    }
return 1;    
}
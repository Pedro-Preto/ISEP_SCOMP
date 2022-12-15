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
#include <signal.h>

#define MY_SHARED_FILE "/Ex_Individual"
#define SEMAFORO_1 0
#define SEMAFORO_2 1
#define SEMAFORO_3 2
#define QUANTIDADE_SEMAFOROS 3
#define QUANTIDADE_FILHOS 3
#define SIZE_ARRAY 5
#define QUANTIDADE_NUMS 100


typedef struct {
    int numbers[SIZE_ARRAY][QUANTIDADE_NUMS];

} shared_data_type;
#define STRUCT_SIZE sizeof(shared_data_type)

void sharedMemoryClosure(int fd,shared_data_type *shared_data);
int spawnChilds(int n);
void semaphoreClosure(sem_t **semaforos,const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80]);

//VERSAO 2 


int main(){
    
    //|=========================|
    //criacao dos semaforos
	const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80] = {"SEM1", "SEM2","SEM3"};   //nome dos semaforos
    const int SEMAFOROS_INITIAL_VALUE[QUANTIDADE_SEMAFOROS] = {1,0,0};               //valores iniciais de cada semaforo   

   sem_t *semaforos[QUANTIDADE_SEMAFOROS] ;                                         //array de semaforos

      // Create semaphore
    for(int i=0;i<QUANTIDADE_SEMAFOROS;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
    //==============================|
	//Criacao da memoria partilhada  	
    
	// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, STRUCT_SIZE);

    // Get a pointer to the data
    shared_data_type *shared_data = (shared_data_type *)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    

    //==============================|
	//Criacao de pipes	
    
    //constantes de leitura e escrita
    const int LEITURA = 0;
    const int ESCRITA = 1; 

    int down[SIZE_ARRAY][2];//pipe bidimensional 

    for(int i=0;i<SIZE_ARRAY;i++){
        if(pipe(down[i])== -1)
        {
            perror("Pipe failed");
            return 1;
        }
    }   

	//====================================================================================================================================
	
    
    srand((unsigned) time(NULL) * getpid());                    //garante que os valores gerados aletoriamente variam
    
    //criar 3 filhos
    int id=spawnChilds(QUANTIDADE_FILHOS);                      //gerar os processos filhos
    
        if(id==1){//=======A1
               
            //carregar os dados para o array de 100 integers
            for(int j=0;j<SIZE_ARRAY;j++){                      //posicao no array bidimensional
                
                sem_wait(semaforos[SEMAFORO_1]);                //processo espera autorizaçao do semaforo  para  avançar
                
                for(int i=0;i<QUANTIDADE_NUMS;i++){
                    shared_data->numbers[j][i]=(rand() % 100)+1; //Valores entre 1 e 100
                }
                
                int time=(rand() % 3);                          //0 e 2 segundos
                sleep(time);                                    //tempo de duracao
                
                sem_post(semaforos[SEMAFORO_2]);                //da autorizaçao do semaforo2 para o processo 2 avançar

            }
            exit(EXIT_SUCCESS);                                 //processo acaba
        }

        if(id==2){//======A2
           for(int j=0;j<SIZE_ARRAY;j++){
               
                sem_wait(semaforos[SEMAFORO_2]);//processo espera autorizaçao do semaforo  para  avançar
               
                for(int i=0;i<QUANTIDADE_NUMS;i++){                                       //100 numeros
                    shared_data->numbers[j][i]=shared_data->numbers[j][i]+10;             //adiconar +10 a todos os elementos
                }
                
                int time=(rand() % 3)+1;                                                  //1 e 3 segundos
                sleep(time);
                
                sem_post(semaforos[SEMAFORO_3]);                                          //da autorizaçao do semaforo3 para o processo 3 avançar

            }
            exit(EXIT_SUCCESS);                                                           //processo acaba

        }
    
        if(id==3){//======A3
            for(int j=0;j<SIZE_ARRAY;j++){
                    
                    sem_wait(semaforos[SEMAFORO_3]);                                                           //processo espera autorizaçao do semaforo  para  avançar
                    
                    close(down[j][LEITURA]);                                                                   //fechar a leitura

                    for(int i=0;i<QUANTIDADE_NUMS;i++){
                        shared_data->numbers[j][i]=shared_data->numbers[j][i]+100;                             //adiconar +10 a todos os elementos
                       /* printf("A1-Array[%d][%d]=%d\n",j,i,shared_data->numbers[j][i]-100-10);
                        printf("A2-Array[%d][%d]=%d\n",j,i,shared_data->numbers[j][i]-100);
                        printf("A3-Array[%d][%d]=%d\n",j,i,shared_data->numbers[j][i]);*/

                        
                        write(down[j][ESCRITA], &shared_data->numbers[j][i],sizeof(shared_data->numbers[j][i]));//escrever no pipe           

                    }

                    close(down[j][ESCRITA]);                                                                    //fechar a escrita
                    
                    int time=(rand() % 3);                                                                      //0 e 2 segundos
                    sleep(time);                                                                                //tempo de durançao do processo    
                 
                    sem_post(semaforos[SEMAFORO_1]);                                                            //da autorizaçao do semaforo1 para o processo 1 avançar

                }
                exit(EXIT_SUCCESS);                                                                             //processo acaba
        }
  

    //===========================================================PAI->P Impressor

 

    printf("===================================\n");
    printf("===================================\n");
    int valor=0;                                                    //Variavel que vai tomar o valor da variavel transportada no pipe
    for(int i=0;i<SIZE_ARRAY;i++){                                  //para cada uma das posições do array bidimensional
        int posicao=0;                                              //posicao de cada valor

        close(down[i][ESCRITA]);                                    //fechar a escrita
        
        while( (read(down[i][LEITURA], &valor, sizeof(valor)))>0){  //enquanto que existirem dados no pipe
            printf("Array[%d][%d]=%d\n",i,posicao,valor);
            posicao++;                                              //posicao de cada valor
        }
        
        close(down[i][LEITURA]);                                    //fechar a leitura
    }
    

    printf("===================================\n");
    printf("===================================\n");

	//===================================================CLOSE SHARED MEMORY AND SEMAPHORES========================================================================
	
    sharedMemoryClosure(fd,shared_data);
    semaphoreClosure(semaforos,SEMAFOROS_NAME);


    return 0;

}

void sharedMemoryClosure(int fd,shared_data_type *shared_data){

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

}

void semaphoreClosure(sem_t **semaforos,const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80]) {
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
}



int spawnChilds(int n){             //codigo para gerar processos filhos
	
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

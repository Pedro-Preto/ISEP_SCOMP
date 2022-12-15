#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define KEYS_SIZE 8000
#define THREADS_NUM 16
#define RANGE KEYS_SIZE/THREADS_NUM
#define CHOOSE_NUM 5
#define MAX_VALUE 49


int keys[KEYS_SIZE][CHOOSE_NUM];

int nums[MAX_VALUE];

int thread_number = 0;

pthread_mutex_t thread_number_update;
pthread_mutex_t numbers_mux[MAX_VALUE];

void* thread_function(void* arg) {

	/* Bloquear o acesso à variavel thread_num para a poder alterar */
	pthread_mutex_lock(&thread_number_update);
	
	/*atualizar numero da thread*/
	int thread_num = thread_number++;
	
	pthread_mutex_unlock(&thread_number_update);
	
	
	for(int i = thread_num * RANGE; i < (thread_num * RANGE)+RANGE; i++) {
		for(int p = 0; p < CHOOSE_NUM; p++) {
			
			/* Num é o numero apostado */
			int num = keys[i][p];
			
			/* Bloquear acesso ao contador de numeros para a poder alterar */
			pthread_mutex_lock(&numbers_mux[num]);
			nums[num]++;
			pthread_mutex_unlock(&numbers_mux[num]);
		}
	}

	pthread_exit((void*)NULL);
}

int main(){
	
    srand((unsigned) time(NULL) * getpid());                    //garante que os valores gerados aletoriamente variam

	/* Preencher o array */
	for(int i = 0; i < KEYS_SIZE; i++){
		for(int p = 0; p < CHOOSE_NUM; p++)
			keys[i][p] = rand() % MAX_VALUE+1;//1 A 49
	}
	/* Criar threads e mutex */
	pthread_t threads[THREADS_NUM];
	pthread_mutex_init(&thread_number_update, NULL);
	
	for(int i = 0; i < MAX_VALUE; i++)
		pthread_mutex_init(&numbers_mux[i], NULL);
	
	for(int i = 0; i < THREADS_NUM; i++) {
		if(pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
			perror("Erro a criar a thread.\n");
			exit(-1);
		}
	}

	
	/* Esperar que as threads terminem */
	for(int i = 0; i < THREADS_NUM; i++){
		if(pthread_join(threads[i], (void*) NULL) != 0){
			perror("Erro a terminar a thread.\n");
			exit(-1);
		}
	}
	/* Terminar mutex */
	pthread_mutex_destroy(&thread_number_update);
	
	for (int i = 0; i < MAX_VALUE; i++)
		pthread_mutex_destroy(&numbers_mux[i]);


	/* Imprimir resultados */
	printf("Numbers:\n");
	for(int i = 0; i < MAX_VALUE; i++)
		printf("Numero %d repetido %d vezes.\n", i + 1, nums[i]);
	
	return 0;

}
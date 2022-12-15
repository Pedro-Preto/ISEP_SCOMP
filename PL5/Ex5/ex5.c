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

#define ARRAY_NUM 1000
#define THREADS_NUM 3
#define MAX_VALUE 100

int array[ARRAY_NUM];
int lowest, highest;
float average;

void* lowest_balance(void *arg) {
	int i;
	/* Instanciar lowest */
	lowest = array[0];
	/* Procurar o menor valor */
	for(i = 0; i < ARRAY_NUM; i++) {
		if(array[i] < lowest)
			lowest = array[i];
	}
	pthread_exit((void*)NULL);
}

void* highest_balance(void *arg) {
	int i;
	/* Instanciar highest */
	highest = array[0];
	/* Procurar o maior valor */
	for(i = 0; i < ARRAY_NUM; i++) {
		if(array[i] > highest)
			highest = array[i];
	}
	pthread_exit((void*)NULL);
}

void* average_balance(void *arg) {
	int i;
	int sum=0;
	/* Calcular a soma de todos os valores */
	for(i = 0; i < ARRAY_NUM; i++){
		sum += array[i];
	}
	/* Calcular a média */
	average = sum/ARRAY_NUM;
	pthread_exit((void*)NULL);
}

int main(){
	
    srand((unsigned) time(NULL) );                    //garante que os valores gerados aletoriamente variam
	/* Preencher array */
	int i;
	for(i = 0; i < ARRAY_NUM; i++)
		array[i] = rand() % MAX_VALUE+1;
	
	pthread_t threads[THREADS_NUM];


    for(int i=0;i<THREADS_NUM;i++){
        if(i==0){
        /* Criar primeira thread (lowest) */
	    pthread_create(&threads[i], NULL, lowest_balance, NULL);
        }
	    if(i==1){
        /* Criar segunda thread (highest) */
        pthread_create(&threads[i], NULL, highest_balance, NULL);
        }
	    /* Criar terceira thread (average) */
	    if(i==2){
        pthread_create(&threads[i], NULL, average_balance, NULL) ;
        }
    }



	/* Terminar todas as threads */
	for(i = 0; i < THREADS_NUM; i++){
		if(pthread_join(threads[i], (void*) NULL) != 0){
			perror("Erro a terminar a thread\n");
			exit(-1);
		}
	}
	/* Imprimir valores */
    printf("==============\n");
	printf("==============\n");
	printf("Lowest: %d\n", lowest);
	printf("Highest: %d\n", highest);
	printf("Average: %.2f\n", average);
	printf("==============\n");
	printf("==============\n");
	return 0;
}
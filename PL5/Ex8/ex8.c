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
#include <limits.h>
#include <pthread.h>
#include <assert.h>

#define MUTEX_NUM 5
#define THREADS_NUM 5
#define ARRAY_SIZE 1000
#define ARRAY_CHUNK 200

pthread_mutex_t mux[MUTEX_NUM];

int data[ARRAY_SIZE];
int result[ARRAY_SIZE];


void* thread_function(void *arg) {


   int index =*((int*)arg);

	for(int d=index*ARRAY_CHUNK;d<(index*ARRAY_CHUNK)+ARRAY_CHUNK;d++){
		result[d] = data[d] * 10 + 2;
	}

	pthread_mutex_lock(&mux[index]);

	for(int e=index*ARRAY_CHUNK;e<(index*ARRAY_CHUNK)+ARRAY_CHUNK;e++){
		printf("result[%d]=%d\n",e,result[e]);
	}

	pthread_mutex_unlock(&mux[index+1]);

	pthread_exit((void*)NULL);
}

int main(){
	
    srand((unsigned) time(NULL) * getpid());                    //garante que os valores gerados aletoriamente variam
	for(int a = 0; a < ARRAY_SIZE; a++){
			data[a] = rand() % 20+1;//1 A 20
	}

	/* Criar threads e mutex */
	pthread_t threads[THREADS_NUM];
	
	for(int b = 0; b < MUTEX_NUM; b++){
		pthread_mutex_init(&mux[b], NULL);
	}
	for(int i = 1; i < MUTEX_NUM; i++){//dar lock a todas as threads menos à primeira
		pthread_mutex_lock(&mux[i]);
	}
	
	int args[THREADS_NUM];
	for(int c = 0; c < THREADS_NUM; c++) {
	//	printf("Index---:%d\n",c);
		args[c]=c;	
		pthread_create(&threads[c], NULL, thread_function, (void*)&args[c]);
	}



	/* Esperar que as threads terminem */
	for(int f = 0; f < THREADS_NUM; f++){
		if(pthread_join(threads[f], (void*) NULL) != 0){
			perror("Erro a terminar a thread.\n");
			exit(-1);
		}
	}
	/* Terminar mutex */
	
	for (int g = 0; g < MUTEX_NUM; g++)
		pthread_mutex_destroy(&mux[g]);



	
	return 0;

}
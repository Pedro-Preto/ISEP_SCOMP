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

#define THREADS_NUM 2
#define MUTEX_NUM 1

int *array;
unsigned int n=0;
int indice=0;

pthread_mutex_t mux[MUTEX_NUM];

void* insertNumber(void *arg) {
  
  printf("Enter any number n:");
  scanf("%d", &n);
  
  pthread_mutex_unlock(&mux[0]);

	pthread_exit((void*)NULL);
}

void* primeNumbersChecker(void *arg) {

	int aux[500];
	pthread_mutex_lock(&mux[0]);

	while(n!=0){
		int c=0;
		for (int i = 1; i <= n; i++) {
			if (n % i == 0) {
				c++;
			}
		}

		if (c == 2) {
			aux[indice]=n;
			indice++;		
		}	
		n--;
	}
	array=aux;
	pthread_exit((void*)NULL);
}



int main(){
	
    srand((unsigned) time(NULL) );                    //garante que os valores gerados aletoriamente variam

	pthread_t threads[THREADS_NUM];

	for(int i=0;i<MUTEX_NUM;i++){
			pthread_mutex_init(&mux[i], NULL);
	}

	/* Criar  threads */
    for(int i=0;i<THREADS_NUM;i++){
        if(i==0){
        	pthread_create(&threads[i], NULL, insertNumber, NULL) ;
			pthread_mutex_lock(&mux[0]);
		}
	    if(i==1){
        	pthread_create(&threads[i], NULL, primeNumbersChecker, NULL) ;
        }
    }



	/* Terminar todas as threads */
	for(int i = 0; i < THREADS_NUM; i++){
		if(pthread_join(threads[i], (void*) NULL) != 0){
			perror("Erro a terminar a thread\n");
			exit(-1);
		}
	}
	/* Terminar todas as mutexes */
  for (int i = 0; i < MUTEX_NUM; i++){
		pthread_mutex_destroy(&mux[i]);
    }


	for(int i=0;i<indice+1;i++){
		printf("%d\n",array[i]);
	}

	return 0;
}
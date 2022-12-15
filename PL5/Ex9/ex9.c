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


#define TRAINS 9
#define CONNECTIONS 3
#define CONNECTION_AB 0
#define CONNECTION_BC 1
#define CONNECTION_BD 2

pthread_mutex_t mux[CONNECTIONS];

void* thread_func(void *arg) {
	int train = *((int*)arg);
	
	srand((unsigned) time(NULL)*getpid());                    //garante que os valores gerados aletoriamente variam

	
	if(train < 3) {
		
		/* Bloquear primeira connection */
		pthread_mutex_lock(&mux[CONNECTION_AB]);
		/* Imprimir informações e simular tempo de viagem */
		int duracao = (rand() % 5)+1;//1 a 5 segundos
		sleep(duracao);
		printf("Train %d - Trajeto: City A para City B - Duracao: %d\n", train, duracao);
		/* Desbloquear connection */
		pthread_mutex_unlock(&mux[CONNECTION_AB]);
		
		/* Bloquear terceira connection */
		pthread_mutex_lock(&mux[CONNECTION_BD]);
		/* Imprimir informações e simular tempo de viagem */
	 	duracao = (rand() % 5)+1;//1 a 5 segundos
		sleep(duracao);
		printf("Train %d - Trajeto: City B para City D - Duracao: %d\n", train, duracao);
		/* Desbloquear connection */
		pthread_mutex_unlock(&mux[CONNECTION_BD]);
		
	} else if(train < 7) {
		
		/* Bloquear segunda connection */
		pthread_mutex_lock(&mux[CONNECTION_BC]);
		/* Imprimir informações e simular tempo de viagem */
		int duracao = (rand() % 5)+1;//1 a 5 segundos
		sleep(duracao);
		printf("Train %d - Trajeto: City B para City C - Duracao: %d\n", train, duracao);
		/* Desbloquear connection */
		pthread_mutex_unlock(&mux[CONNECTION_BC]);
		
	} else {
		
		/* Bloquear terceira connection */
		pthread_mutex_lock(&mux[CONNECTION_BD]);
		/* Imprimir informações e simular tempo de viagem */
		int duracao = (rand() % 5)+1;//1 a 5 segundos
		sleep(duracao);
		printf("Train %d - Trajeto: City B para City D - Duracao: %d\n", train, duracao);
		/* Desbloquear connection */
		pthread_mutex_unlock(&mux[CONNECTION_BD]);
	
	}
	pthread_exit(NULL);
}

int main() {
	int i;
	/* Criar as threads e o mutex*/
	pthread_t threads[TRAINS];
	
	for (i = 0; i < CONNECTIONS; i++)
		pthread_mutex_init(&mux[i], NULL);

	int args[TRAINS];

	for (i = 0; i < TRAINS; i++) {
		 args[i] = i; 
        pthread_create(&threads[i], NULL, thread_func, (void*)&args[i]);
	}

	/* Esperar que se as threads terminem */
	for(i = 0; i < TRAINS; i++)
		pthread_join(threads[i], NULL);

	/* Eliminar mutex */
	for (i = 0; i < CONNECTIONS; i++)
		pthread_mutex_destroy(&mux[i]);

	return 0;
}
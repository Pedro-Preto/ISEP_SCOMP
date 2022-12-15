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

#define NEGATIVA 50
#define NUM_PROVAS 300
#define NUM_THREADS 5
#define POS 0
#define NEG 1

typedef struct {
	int number; 
	int notaG1;
	int notaG2;
	int notaG3;
	int notaFinal;
} Prova;

Prova provas[NUM_PROVAS];
int num[2] = {0, 0}, aux[2];
pthread_mutex_t	mux[2], prova[NUM_PROVAS];
pthread_cond_t cond[2];

void* T1(void* arg){//a primeira thread vai criar as provas
	
	/* Criar 300 provas com valores aleatorios para as notas das 3 provas */
	
	
	for(int i = 0; i < NUM_PROVAS; i++){
		provas[i].notaG1 = rand() % 100 + 1;
		provas[i].notaG2 = rand() % 100 + 1;
		provas[i].notaG3 = rand() % 100 + 1;
		
		/* Desbloquear as mutex prova para esta prova, uma vez que ja se terminou a sua escrita */
		pthread_mutex_unlock(&prova[i]);	
	}
	pthread_exit(NULL);
}

void* T2E3(void* arg){
	int par_ou_impar = *((int *)arg);	

	/* Vai calcular apenas para as provas com indice par, ou com indice impar */
	for(int i = par_ou_impar; i < NUM_PROVAS; i+=2) {
	
		/* Bloquear mutex para as prova desta prova */
		pthread_mutex_lock(&prova[i]);
		
		/* Calcular a nota final */
		provas[i].notaFinal = (provas[i].notaG1 + provas[i].notaG2 + provas[i].notaG3) / 3;
	
		/* Se a nota final for negativa */
		if(provas[i].notaFinal < NEGATIVA) {
			/* Bloquear mutex negativa */
			pthread_mutex_lock(&mux[NEG]);//T5
			/* Aumentar contador negativa */
			aux[NEG]++;
			/* Desbloquear mutex negativa */
			pthread_mutex_unlock(&mux[NEG]);
			/* Ativar condição negativa */
			pthread_cond_signal(&cond[NEG]);
		/* Se a nota final for positiva */			
		} else {
			/* Bloquear mutex positiva */
			pthread_mutex_lock(&mux[POS]);//T4
			/* Aumentar contador positiva */
			aux[POS]++;
			/* Desbloquear mutex positiva */
			pthread_mutex_unlock(&mux[POS]);
			/* Ativar condição positiva */
			pthread_cond_signal(&cond[POS]);
		}
	}
	pthread_exit(NULL);
}

void* T4(void *arg) {
	/* enquanto nao analisou todas as provas */
	while(num[NEG] + num[POS] < NUM_PROVAS) {
		pthread_mutex_lock(&mux[POS]);

		while(aux[POS] == 0){/* Esperar enquanto nao ha provas positivas */
			pthread_cond_wait(&cond[POS], &mux[POS]);
		}
		/* Quando o numero de provas chegar ao seu valor maximo, terminar a thread */ 
		if (num[NEG] + num[POS] >= NUM_PROVAS) {
			pthread_mutex_unlock(&mux[POS]);
			break;
		}
		/* Se a condição for ativada, incrementar num */
		num[POS]++;
		aux[POS]--;
		pthread_mutex_unlock(&mux[POS]);
	}

	/** Força o T5 a sair do pthread_cond_wait quando ja nao houver mais provas 
		deste tipo (condição negativa pode estar parada infinitamente) */
	aux[NEG]++;
	pthread_cond_signal(&cond[NEG]);//dá sinal a T5
	pthread_exit(NULL);
}

void* T5(void *arg) {
	/* enquanto nao analisou todas as provas */
	while(num[NEG] + num[POS] < NUM_PROVAS) {
		pthread_mutex_lock(&mux[NEG]);
		/* Esperar enquanto nao ha provas negativas */
		while(aux[NEG] == 0){
			pthread_cond_wait(&cond[NEG], &mux[NEG]);
		}
		/* Quando o numero de provas chegar ao seu valor maximo, terminar a thread */
		if (num[NEG] + num[POS] >= NUM_PROVAS) {
			pthread_mutex_unlock(&mux[NEG]);
			break;
		}
		/* Se a condição for ativada, incrementar num */
		num[NEG]++;
		aux[NEG]--;
		
		pthread_mutex_unlock(&mux[NEG]);
	}
	/** Força o T4 a sair do pthread_cond_wait quando ja nao houver mais provas 
		deste tipo (condição positiva pode estar parada infinitamente) */
	aux[POS]++;
	pthread_cond_signal(&cond[POS]);
	pthread_exit(NULL);
}

int main() {
	int i;
	srand(time(NULL));
	/* Criar metodos de sincronizacao - mutex e cond */
	/* Criar mutex para cada prova */
	for (i = 0; i < NUM_PROVAS; i++) {
		pthread_mutex_init(&prova[i], NULL);
		pthread_mutex_lock(&prova[i]);
	}
	/* Criar mutex para pos e neg */
	for(i = 0; i < 2; i++) {
		pthread_cond_init(&cond[i], NULL);
		pthread_mutex_init(&mux[i], NULL);
	}

	pthread_t thread[NUM_THREADS];

	/* Criar as threads e iniciar thread 1 */
	pthread_create(&thread[0],NULL, T1, NULL);
	
	/* Criar thread 2 e 3 */
	int par = 0;
	pthread_create(&thread[1],NULL, T2E3, (void*)&par);
	int impar = 1;
	pthread_create(&thread[2],NULL, T2E3, (void*)&impar);
	
	/* Criar threads 4 e 5 */
	pthread_create(&thread[3],NULL, T4, NULL);
	pthread_create(&thread[4],NULL, T5, NULL);
	
	/* Terminar todas as threads */
	for (i = 0; i < NUM_THREADS; i++) 
		pthread_join(thread[i],NULL);	
	/* Terminar todos os mutex e condiçoes */
	for (i = 0; i < NUM_PROVAS; i++) 
		pthread_mutex_destroy(&prova[i]);
	for(i = 0; i < 2; i++) {
		pthread_mutex_destroy(&mux[i]);
		pthread_cond_destroy(&cond[i]);
	}
	/* Imprimir resultados */
	printf("Notas positivas: %.0f \nNotas negativas: %.0f\n", ((float)num[POS]/NUM_PROVAS)*100, ((float)num[NEG]/NUM_PROVAS)*100);

	return 0;
}

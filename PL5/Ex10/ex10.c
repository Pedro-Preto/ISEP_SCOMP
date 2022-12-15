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
#define THREADS_NUM 6
#define ARRAY_SIZE 10000

#define THREAD_1 0
#define THREAD_2 1
#define THREAD_3 2
#define THREAD_4 3
#define THREAD_5 4
#define THREAD_6 5

#define THREAD_1_AUTHORIZATION 0
#define THREAD_2_AUTHORIZATION 1
#define THREAD_3_AUTHORIZATION 2
#define THREAD_4_AUTHORIZATION_5 3
#define THREAD_5_AUTHORIZATION_6 4

typedef struct {
    char id_h[50];
    char id_p[50];
    float price_p;
} Data;

typedef struct {
    char id_h[50];
    float price_p;
} DataLowest;

#define TOTAL_PRODUTOS 5
#define TOTAL_SUPERMECADOS 3

char supermercados[TOTAL_SUPERMECADOS][80]={"Pingo Doce","Continente","Lidle"};
char produtos[TOTAL_PRODUTOS][80]={"Bananas","Pessegos","Morangos","Cerejas","Melancias"};
float precos_pingo_doce[TOTAL_PRODUTOS]={2.4,2.9,3,4,5.4};
float precos_continente[TOTAL_PRODUTOS]={2.9,2.5,3.5,3.4,4.4};
float precos_lidle[TOTAL_PRODUTOS]={2.8,2.5,3.2,3,3.4};

pthread_mutex_t mux[MUTEX_NUM];

Data Vec[ARRAY_SIZE];
Data Vec1[ARRAY_SIZE];
Data Vec2[ARRAY_SIZE];
Data Vec3[ARRAY_SIZE];

int indexVec1=0;
int indexVec2=0;
int indexVec3=0;

DataLowest lowestData;

//=============================================
//=============================================

void* T1(void *arg) {

	for(int i =0;i<ARRAY_SIZE;i++){
		if(strcmp(Vec[i].id_h,supermercados[0])==0){
			strcpy(Vec1[indexVec1].id_h,Vec[i].id_h);
			strcpy(Vec1[indexVec1].id_p,Vec[i].id_p);
			Vec1[indexVec1].price_p=Vec[i].price_p;
			indexVec1++;
		}
	}
	pthread_mutex_unlock(&mux[THREAD_1_AUTHORIZATION]);

	pthread_exit((void*)NULL);
}


void* T2(void *arg) {

	for(int i =0;i<ARRAY_SIZE;i++){
		if(strcmp(Vec[i].id_h,supermercados[1])==0){
			strcpy(Vec2[indexVec2].id_h,Vec[i].id_h);
			strcpy(Vec2[indexVec2].id_p,Vec[i].id_p);
			Vec2[indexVec2].price_p=Vec[i].price_p;
			indexVec2++;
		}
	}
	pthread_mutex_unlock(&mux[THREAD_2_AUTHORIZATION]);

	pthread_exit((void*)NULL);
}

void* T3(void *arg) {

	for(int i =0;i<ARRAY_SIZE;i++){
		if(strcmp(Vec[i].id_h,supermercados[2])==0){
			strcpy(Vec3[indexVec3].id_h,Vec[i].id_h);
			strcpy(Vec3[indexVec3].id_p,Vec[i].id_p);
			Vec3[indexVec3].price_p=Vec[i].price_p;
			indexVec3++;
		}
	}
	pthread_mutex_unlock(&mux[THREAD_3_AUTHORIZATION]);




	pthread_exit((void*)NULL);
}


void* T4(void *arg) {
													//quantidade de produtos-indice 0      preço dos produtos-indice 1     
	float produtos_preco[TOTAL_PRODUTOS]={0,0,0,0,0}; //0-Bananas,1-Pessegos,2-Morangos,3-Cerejas,4-Melancias
	int i=0;

	while(produtos_preco[0]==0 ||produtos_preco[1]==0 ||produtos_preco[2]==0 ||produtos_preco[3]==0 ||produtos_preco[4]==0 )
	{
		if(strcmp(Vec1[i].id_p,produtos[0])==0){
			produtos_preco[0]=Vec1[i].price_p;
		}

		if(strcmp(Vec1[i].id_p,produtos[1])==0){
			produtos_preco[1]=Vec1[i].price_p;
		}

		if(strcmp(Vec1[i].id_p,produtos[2])==0){
			produtos_preco[2]=Vec1[i].price_p;
		}

		if(strcmp(Vec1[i].id_p,produtos[3])==0){
			produtos_preco[3]=Vec1[i].price_p;
		}

		if(strcmp(Vec1[i].id_p,produtos[4])==0){
			produtos_preco[4]=Vec1[i].price_p;
		}
	i++;
	}
	
	/*for(int i=0;i<TOTAL_PRODUTOS;i++){
	
		printf("Produto:%s Preço:%.2f\n",produtos[i],produtos_preco[i]);
	}*/
	float average=0;

	for (int i = 0; i < TOTAL_PRODUTOS; i++)
	{
		average+=produtos_preco[i];
	}
	
	average/=TOTAL_PRODUTOS;

	printf("\nHypermarket:%s, average oF All Products:%.2f\n",Vec1[0].id_h,average);

	//atualizar a variavel global
	strcpy(lowestData.id_h,Vec1[0].id_h);

	lowestData.price_p=average;

//dar allow ao t5 para checkar se é menor
	pthread_mutex_unlock(&mux[THREAD_4_AUTHORIZATION_5]);


	pthread_exit((void*)NULL);
}


void* T5(void *arg) {
												//quantidade de produtos-indice 0      preço dos produtos-indice 1     
	float produtos_preco[TOTAL_PRODUTOS]={0,0,0,0,0}; //0-Bananas,1-Pessegos,2-Morangos,3-Cerejas,4-Melancias
	int i=0;

	while(produtos_preco[0]==0 ||produtos_preco[1]==0 ||produtos_preco[2]==0 ||produtos_preco[3]==0 ||produtos_preco[4]==0 )
	{
		if(strcmp(Vec2[i].id_p,produtos[0])==0){
			produtos_preco[0]=Vec2[i].price_p;
		}

		if(strcmp(Vec2[i].id_p,produtos[1])==0){
			produtos_preco[1]=Vec2[i].price_p;
		}

		if(strcmp(Vec2[i].id_p,produtos[2])==0){
			produtos_preco[2]=Vec2[i].price_p;
		}

		if(strcmp(Vec2[i].id_p,produtos[3])==0){
			produtos_preco[3]=Vec2[i].price_p;
		}

		if(strcmp(Vec2[i].id_p,produtos[4])==0){
			produtos_preco[4]=Vec2[i].price_p;
		}
	i++;
	}

	
/*	for(int i=0;i<TOTAL_PRODUTOS;i++){
	
		printf("Produto:%s Preço:%.2f\n",produtos[i],produtos_preco[i]);
	}*/
	float average=0;

	for (int i = 0; i < TOTAL_PRODUTOS; i++)
	{
		average+=produtos_preco[i];
	}
	
	average/=TOTAL_PRODUTOS;

	printf("\nHypermarket:%s, average oF All Products:%.2f\n",Vec2[0].id_h,average);

	pthread_mutex_lock(&mux[THREAD_4_AUTHORIZATION_5]);

	if(average<lowestData.price_p){
		strcpy(lowestData.id_h,Vec2[0].id_h);
		lowestData.price_p=average;
	}

	pthread_mutex_unlock(&mux[THREAD_5_AUTHORIZATION_6]);

	pthread_exit((void*)NULL);
}


void* T6(void *arg) {

float produtos_preco[TOTAL_PRODUTOS]={0,0,0,0,0}; //0-Bananas,1-Pessegos,2-Morangos,3-Cerejas,4-Melancias
	int i=0;

	while(produtos_preco[0]==0 ||produtos_preco[1]==0 ||produtos_preco[2]==0 ||produtos_preco[3]==0 ||produtos_preco[4]==0 )
	{
		if(strcmp(Vec3[i].id_p,produtos[0])==0){
			produtos_preco[0]=Vec3[i].price_p;
		}

		if(strcmp(Vec3[i].id_p,produtos[1])==0){
			produtos_preco[1]=Vec3[i].price_p;
		}

		if(strcmp(Vec3[i].id_p,produtos[2])==0){
			produtos_preco[2]=Vec3[i].price_p;
		}

		if(strcmp(Vec3[i].id_p,produtos[3])==0){
			produtos_preco[3]=Vec3[i].price_p;
		}

		if(strcmp(Vec3[i].id_p,produtos[4])==0){
			produtos_preco[4]=Vec3[i].price_p;
		}
		i++;
	}
	
	/*for(int i=0;i<TOTAL_PRODUTOS;i++){
	
		printf("Produto:%s Preço:%.2f\n",produtos[i],produtos_preco[i]);
	}*/
	float average=0;

	for (size_t i = 0; i < TOTAL_PRODUTOS; i++)
	{
		average+=produtos_preco[i];
	}
	average/=TOTAL_PRODUTOS;

	printf("\nHypermarket:%s, average oF All Products:%.2f\n",Vec3[0].id_h,average);

	pthread_mutex_lock(&mux[THREAD_5_AUTHORIZATION_6]);
	if(average<lowestData.price_p){
		strcpy(lowestData.id_h,Vec3[0].id_h);
		lowestData.price_p=average;
	}

	pthread_exit((void*)NULL);
}

int main(){
	
    srand((unsigned) time(NULL) * getpid());                    //garante que os valores gerados aletoriamente variam
	
	
	for(int a = 0; a < 3333; a++){//pingo doce
		strcpy(	Vec[a].id_h ,supermercados[0]);//0 a 2
		int index=rand() % 5;//0 a 4
		strcpy(Vec[a].id_p,produtos[index]);
		Vec[a].price_p=precos_pingo_doce[index]; 
	}

	for(int a = 3333; a < 6666; a++){//continente
		strcpy(	Vec[a].id_h ,supermercados[1]);//0 a 2
		int index=rand() % 5;//0 a 4
		strcpy(Vec[a].id_p,produtos[index]);
		Vec[a].price_p=precos_continente[index]; 
	}

	for(int a = 6666; a <ARRAY_SIZE; a++){//lidle
		strcpy(	Vec[a].id_h ,supermercados[2]);//0 a 2
		int index=rand() % 5;//0 a 4
		strcpy(Vec[a].id_p,produtos[index]);
		Vec[a].price_p=precos_lidle[index]; 
		}


	for(int b = 0; b < MUTEX_NUM; b++){
			pthread_mutex_init(&mux[b], NULL);
			pthread_mutex_lock(&mux[b]);

		}
		
	/* Criar threads e mutex */
	pthread_t threads[THREADS_NUM];

	pthread_create(&threads[THREAD_1], NULL, T1, NULL);
	pthread_create(&threads[THREAD_2], NULL, T2, NULL);
	pthread_create(&threads[THREAD_3], NULL, T3, NULL);

	pthread_mutex_lock(&mux[THREAD_1_AUTHORIZATION]);
	pthread_mutex_lock(&mux[THREAD_2_AUTHORIZATION]);
	pthread_mutex_lock(&mux[THREAD_3_AUTHORIZATION]);

	pthread_create(&threads[THREAD_4], NULL, T4, NULL);
	pthread_create(&threads[THREAD_5], NULL, T5, NULL);
	pthread_create(&threads[THREAD_6], NULL, T6, NULL);

	
	

	/* Esperar que as threads terminem */
	for(int f = 0; f < THREADS_NUM; f++){
		if(pthread_join(threads[f], (void*) NULL) != 0){
			perror("Erro a terminar a thread.\n");
			exit(-1);
		}
	}
	/* Terminar mutex */
	
	for (int g = 0; g < MUTEX_NUM; g++){
		pthread_mutex_destroy(&mux[g]);
	}
	printf("\nHypermaket:%s, Final Average Price:%.2f\n",lowestData.id_h,lowestData.price_p);


	
	return 0;

}
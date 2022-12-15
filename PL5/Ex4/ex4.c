#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
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

#define SIZE_MATRIX 8
#define THREADS_FILL_NUM 2
#define THREADS_NUM 8
 
int matrix1[SIZE_MATRIX][SIZE_MATRIX];
int matrix2[SIZE_MATRIX][SIZE_MATRIX];
int matrixResult[SIZE_MATRIX][SIZE_MATRIX];

int linha=0;

pthread_mutex_t mux[THREADS_FILL_NUM];

void* fill_matrix1(void *arg) {

        for(int i=0;i<SIZE_MATRIX;i++){
            for(int j=0;j<SIZE_MATRIX;j++){
                matrix1[i][j]=(rand() % 10)+1;
            }
        }
        
        printf("-                       -\n");
        for(int i=0;i<SIZE_MATRIX;i++){
            printf("|%d,%d,%d,%d,%d,%d,%d,%d|\n",matrix1[i][0],matrix1[i][1],matrix1[i][2],matrix1[i][3],matrix1[i][4],matrix1[i][5],matrix1[i][6],matrix1[i][7]);
        }
        printf("-                       -\n");
   		pthread_mutex_unlock(&mux[0]);
	
    pthread_exit((void*)NULL);
}

void* fill_matrix2(void *arg) {

         for(int i=0;i<SIZE_MATRIX;i++){
            for(int j=0;j<SIZE_MATRIX;j++){
                matrix2[i][j]=(rand() % 10)+1;
            }
        }
           
        printf("-                       -\n");
        for(int i=0;i<SIZE_MATRIX;i++){
            printf("|%d,%d,%d,%d,%d,%d,%d,%d|\n",matrix2[i][0],matrix2[i][1],matrix2[i][2],matrix2[i][3],matrix2[i][4],matrix2[i][5],matrix2[i][6],matrix2[i][7]);
        }
        printf("-                       -\n");
		pthread_mutex_unlock(&mux[1]);

	pthread_exit((void*)NULL);
}


void* multiply_matrix(void *arg) {

   // int linha =*((int*)arg);

		pthread_mutex_lock(&mux[0]);
		pthread_mutex_lock(&mux[1]);

      for (int coluna2 = 0; coluna2 < SIZE_MATRIX; coluna2++) {
         for (int coluna1 = 0; coluna1 < SIZE_MATRIX; coluna1++) {
            matrixResult[linha][coluna2] += matrix1[linha][coluna1] * matrix2[coluna1][coluna2];
         }
       //   printf("%d (%d,%d),",matrixResult[linha][coluna2],linha,coluna2);

      }
   //   printf("\n");
      linha++;
		pthread_mutex_unlock(&mux[0]);
		pthread_mutex_unlock(&mux[1]);

    pthread_exit((void*)NULL);
    
}




int main(){
    srand((unsigned) time(NULL));                    //garante que os valores gerados aletoriamente variam


    pthread_t threadsFill[THREADS_FILL_NUM];

    pthread_t threads[THREADS_NUM];
    
	for(int i=0;i<THREADS_FILL_NUM;i++){
        pthread_mutex_init(&mux[i], NULL);
    }

	pthread_create(&threadsFill[0], NULL, fill_matrix1, NULL);
    pthread_create(&threadsFill[1], NULL, fill_matrix2, NULL);
        
		pthread_mutex_lock(&mux[0]);
		pthread_mutex_lock(&mux[1]);
	
    for(int i=0;i<THREADS_NUM;i++){
        	pthread_create(&threads[i], NULL, multiply_matrix, (void*)&i);
    }

	

	/* Esperar que as threads terminem */
   for(int i = 0; i < THREADS_FILL_NUM; i++){
		if(pthread_join(threadsFill[i], (void*) NULL) != 0){
			perror("Erro a terminar a thread.\n");
			exit(-1);
		}
	}
    
    for(int i = 0; i < THREADS_NUM; i++){
		if(pthread_join(threads[i], (void*) NULL) != 0){
			perror("Erro a terminar a thread.\n");
			exit(-1);
		}
	}

	for (int i = 0; i < THREADS_FILL_NUM; i++){
		pthread_mutex_destroy(&mux[i]);
    }

    
    printf("-                       -\n");
    
    for(int i=0;i<SIZE_MATRIX;i++){
            printf("|%d,%d,%d,%d,%d,%d,%d,%d|\n",matrixResult[i][0],matrixResult[i][1],matrixResult[i][2],matrixResult[i][3],matrixResult[i][4],matrixResult[i][5],matrixResult[i][6],matrixResult[i][7]);

    }
    
    printf("-                       -\n");


    
}





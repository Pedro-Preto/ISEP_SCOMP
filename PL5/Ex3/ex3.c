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

#define SIZE_OF_ARRAY 1000
#define SIZE_OF_EACH_CHUNK 100
#define NUMBER_OF_THREADS 10
void* searchAndPrint(void* arg) ;

typedef struct {
    int numberToSearch;
    int indexOfThread;
    int array[SIZE_OF_ARRAY];
} Data;

typedef struct{
int a;
}my_struct;


int main(){
  
	//Threads array
 	pthread_t threads[NUMBER_OF_THREADS];
    
	// Data for threads result
    Data threadData[NUMBER_OF_THREADS];
    
    // Number to search for
    int numberToSearch;

    // Ask for the number to search the array
    printf("Enter a number to search the array:\n");
    scanf("%d", &numberToSearch);
    printf("\n");

    // Fill the array with unique numbers
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
		threadData[i].indexOfThread=i;
		threadData[i].numberToSearch = numberToSearch;
		for(int j=0;j<SIZE_OF_ARRAY;j++){
        threadData[i].array[j] = j;
		}
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        if(pthread_create(&threads[i], NULL, searchAndPrint, (void*)&threadData[i])!=0){
			perror("Erro a criar thread\n");
			exit(-1);
		}
	}
    for (int j = 0; j < NUMBER_OF_THREADS; j++) {

		void *ret;
	//	my_struct values;
        pthread_join(threads[j], (void*)&ret);
	//	values = *((my_struct*)ret);
		free(ret);
       
	   if(ret!=0){
	    printf("A thread %d finished with the return value of: %d\n", j+1,ret);
		printf("===================================================\n");

    	}
	}

    return EXIT_SUCCESS;

}

void* searchAndPrint(void* arg) {

    Data* argumento = (Data*)arg;
//	my_struct *ms = (my_struct*)malloc(sizeof(my_struct));
   
    int numberToSearch = argumento->numberToSearch;
    int indexOfThread = argumento->indexOfThread;
    int* array = argumento->array;


    // Search the array
    for (int k = indexOfThread*100; k < (indexOfThread*100)+100; k++) {
        if (array[k] == numberToSearch) {
		printf("===================================================\n");
            printf("The number %d with index %d was found by thread number %d\n", array[k],k, indexOfThread+1);
			//ms->a=indexOfThread+1;
            pthread_exit((void*)&indexOfThread+1);
        }
    }
	//ms->a=-1;

    pthread_exit((void*)NULL);
}
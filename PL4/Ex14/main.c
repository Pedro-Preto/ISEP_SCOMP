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

#define MY_SHARED_FILE "/Ex_14"
#define QUANTIDADE_READERS 4
#define QUANTIDADE_WRITERS 7



typedef struct{
    char string[100];
    int currentReaders;
    int nReaders;
    int nWriters;
} shared_memory_area;

#define STRUCT_SIZE sizeof(shared_memory_area)

void sharedMemoryClosure(int fd,shared_memory_area *shared_data);
int spawnChilds(int n);



int main(){

	//Criacao da memoria partilhada  	
    
	// Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
	// Defines the size
    ftruncate(fd, STRUCT_SIZE);

    // Get a pointer to the data
    shared_memory_area *shared_data = (shared_memory_area *)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //===============================Correr writter e readers 


    int id=spawnChilds(QUANTIDADE_READERS+QUANTIDADE_WRITERS); 
    if(id>0){
        for(int i=0; i<QUANTIDADE_WRITERS+QUANTIDADE_READERS; i++){
            
            if(id==i+1){
                if(id<=QUANTIDADE_WRITERS){
                    execlp("./writter.bin","./writter.bin",(char*)NULL);
                    //printf("%d\n",id);
                    //exit(0);
                }else{
                    execlp("./reader.bin","./reader.bin",(char*)NULL);
                    //printf("%d\n",id);
                    //exit(0);
                }
            }  
        }
    exit(0);
    }
      
    while (wait(NULL) > 0) {
        // wait until all children are dead
    };
    
	//===================================================CLOSE SHARED MEMORY AND SEMAPHORES========================================================================
	
    sharedMemoryClosure(fd,shared_data);


    return 0;

}

void sharedMemoryClosure(int fd,shared_memory_area *shared_data){

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

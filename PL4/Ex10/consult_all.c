//Lists all data in the shared memory area. You should only display the records that contain valid data.

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h> 
#include <time.h>  
#define MY_SHARED_FILE "/ex10shm"
#define SEMAFORO "/ex10shm"

typedef struct{
    char name[10];
    int number;
    char address[20];
} users_info;

typedef struct {
    users_info logins[100];
    int index;
}loginsQtd;
#define STRUCT_SIZE sizeof(loginsQtd)

int main(){

    int fd;
    sem_t *semaphore;
 
    loginsQtd *loginStruct;

    fd = shm_open(MY_SHARED_FILE, O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1){
        printf("Error while creating shared memory\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(fd, STRUCT_SIZE);
    loginStruct = (loginsQtd*)mmap(NULL, STRUCT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if((semaphore = sem_open(SEMAFORO, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){ //exclusao mutua
        semaphore = sem_open(SEMAFORO, O_CREAT);
    }


    // verifica se pode aceder à secção crítica 
    sem_wait(semaphore);

    /* lista toda a info dos users registados */
    int i;
    for(i = 0; i < loginStruct->index; i++){
        printf("### Utilizador %d: ###\n", i+1);
        printf("Nome: %s;\n", loginStruct->logins[i].name);
        printf("Número: %d;\n", loginStruct->logins[i].number);
        printf("Morada: %s.\n", loginStruct->logins[i].address);
        fflush(0);
    }

    sem_post(semaphore);
    
   
   // Undo mapping
    if (munmap(loginStruct, STRUCT_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

    
    return 0;
}
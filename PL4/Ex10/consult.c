//to consult a user’s personal data. To do this, you must ask the user for his or her identification number

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
    int number;
 
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


    printf("Number id:\n");
    scanf("%d", &number);//num do user
     
    for(int p=0;p<loginStruct->index;p++){
       if(loginStruct->logins[p].number==number){
        
        printf("=============================\n");
        
        printf("Informação do utilizador:\n");
        printf("Número: %d;\n", loginStruct->logins[p].number);
        printf("Nome: %s;\n", loginStruct->logins[p].name);
        printf("Morada: %s.\n", loginStruct->logins[p].address);

        printf("=============================\n");
       }
    }
        
    sem_post(semaphore); //incrementa



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
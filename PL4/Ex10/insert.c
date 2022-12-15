//Allows you to enter a user’s data in the shared memory area.

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
        printf("Index:%d\n",loginStruct->index);


    if((semaphore = sem_open(SEMAFORO, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){ //semaforo exclusao mutua um de cada vez
        semaphore = sem_open(SEMAFORO, O_CREAT);
    }

    // verifica se pode aceder à secção crítica, decrementa o valor
    sem_wait(semaphore);

    // pede as informções ao utilizador e guarda-as na posicao e dentro da posicao na variavel correspondente do users
    printf("Insira o nº de utilizador:\n");
    scanf("%d", &loginStruct->logins[ loginStruct->index].number);

    printf("Insira o nome do utilizador:\n");
    scanf("%s", loginStruct->logins[ loginStruct->index].name);


    printf("Insira a morada do utilizador:\n");
    scanf("%s", loginStruct->logins[ loginStruct->index].address);


    loginStruct->index+=1; //avança com o index para a próxima posição do vetor
        printf("Index:%d\n",loginStruct->index);

    sem_post(semaphore); //incrementa o valor para voltar o sem a 1 
   


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
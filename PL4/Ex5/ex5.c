#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>


#define SEMAPHORE "/pl4ex5_sem"

int main(){

sem_t *semaforo;
      // Create semaphore
    if ((semaforo = sem_open(SEMAPHORE, O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    pid_t p=fork();

        if (p == 0) /* Child process */
        {
            printf("I am the child!\n");
            sem_post(semaforo);
        exit(EXIT_SUCCESS);
        }else if(p>0){ /* Parent process */
            sem_wait(semaforo);
            printf("I am the father!\n");
        }
    
    sem_close(semaforo);
    sem_unlink(SEMAPHORE);

return 0;
}
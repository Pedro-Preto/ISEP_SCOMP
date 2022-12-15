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


// Name of shared memory area
#define MEM_AREA "/pl4ex3_shared_mem_area"

// String to be added to memory area
#define STR "I'm the Father - with PID "
// Minimum number of seconds to wait

#define SEMAPHORE "/pl4ex3_sem"
// Number of strings
#define NUM_STRINGS 50
// Number of characters in each string
#define NUM_CHARS 85

// Shared memory area with array of 50 strings of 80 characters
typedef struct{
    int index;
    char s[NUM_STRINGS][NUM_CHARS];
}shared_data_type;


int main(){

  int fd;
  sem_t *sem;
  shared_data_type *memory;

    // Use current time as seed for random generator
   // srand(time(0));

    // Create memory area
    fd = shm_open(MEM_AREA, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(shared_data_type));
    memory = (shared_data_type *)mmap(NULL, sizeof(shared_data_type), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Create semaphore
    if ((sem = sem_open(SEMAPHORE, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    // Create 5 processes
  int i = 0;
  pid_t p;
    do
    {
        p = fork();
        i++;
    
    } while (p > 0 && i < 8);

        if (p == 0) /* Child process */
        {
            // Block semaphore
            sem_wait(sem);

            // Writing in memory area
            sprintf(memory->s[memory->index], "%s%d", STR, getpid());

            // Increasing the index
            memory->index++;
            if (memory->index >= NUM_STRINGS)
            {
                memory->index = 0;
            }

            // Raise semaphore
            sem_post(sem);

            // Waiting random number between 1 and 5 seconds
            sleep((rand() % 5) + 1);

            // Close semaphore
            sem_close(sem);

            exit(EXIT_SUCCESS);
        }

        wait(NULL);

    for (i = 0; i < memory->index; i++)
    {
        printf("%s\n", memory->s[i]);
    }

    // Unlink semaphore
    sem_unlink(SEMAPHORE);

    // Undo mapping
    if (munmap((void *)memory, sizeof(shared_data_type)) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

    // Remove file from system
    if (shm_unlink(MEM_AREA) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }


return 0;
}
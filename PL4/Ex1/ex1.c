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


// Number of numbers to be used in the files
#define INPUT_FILE "Numbers.txt"
// Name of file to write
#define OUTPUT_FILE "Output.txt"
// Name of semaphore
#define SEMAPHORE "/pl4ex1_sem"

int cria_filhos(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main(){

    FILE *inputFile;
    FILE *outputFile;
    sem_t *sem;
    int num;

    // Create semaphore to close and clean
    if ((sem = sem_open(SEMAPHORE, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

     // Create clean output file
    outputFile = fopen(OUTPUT_FILE, "w");
    fclose(outputFile);

    int p=cria_filhos(8);

    //Processo Filho
    if(p==0){
      sem_wait(sem);

        // Open input file for reading
        inputFile = fopen(INPUT_FILE, "r");
        if (inputFile != NULL)
        {
            // Open output file for writing at the end of the file
            outputFile = fopen(OUTPUT_FILE, "a");

            for (int j = 0; j < 200; j++)
            {
                fscanf(inputFile, "%d", &num);
                fprintf(outputFile, "%d\n", num);
            }

            // Close output file
            fclose(outputFile);

            // Close input file
            fclose(inputFile);
        }
        sem_post(sem);
        exit(EXIT_SUCCESS);    

    //Processo Pai
    }

    //Espera o término de todos os processos
    for(int i=0;i<8;i++){
        wait(NULL);
    }

 // Open output file for reading
    outputFile = fopen(OUTPUT_FILE, "r");
    if (outputFile == NULL)
    {
        printf("Could not open file");
    }
    else
    {
        while (fscanf(outputFile, "%d", &num) != EOF)
        {
            printf("%d\n", num);
        }
    }
    fclose(outputFile);

    sem_close(sem);
    sem_unlink(SEMAPHORE);

return 0;
}
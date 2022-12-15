#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define NUM_CHILDS 8
#define NUM_FILENAME "Numbers.txt"
#define OUT_FILENAME "Output.txt"

int main() {
    int i;
    sem_t *sem[NUM_CHILDS];
    char sem_name[8][7];
    for (i = 0; i < NUM_CHILDS; i++) 
        sprintf(sem_name[i], "/sema%d", i);

    if ((sem[0] = sem_open(sem_name[0], O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    for (i = 1; i < NUM_CHILDS; i++) {
        if ((sem[i] = sem_open(sem_name[i], O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(1);
        }
    }

    FILE *in, *out;
    if ((out = fopen(OUT_FILENAME, "w")) == NULL) return 1; // Erro ao abrir ficheiro
    fclose(out); // Limpar ficheiro de output
    int j;
    pid_t p;
    for (i = 0; i < NUM_CHILDS; i++) {
        p = fork();
        if (p < 0) return 1; // Erro ao fazer fork
        if (p == 0) {
            if ((in = fopen(NUM_FILENAME, "r")) == NULL) return 1; // Erro ao abrir ficheiro
            if ((out = fopen(OUT_FILENAME, "a")) == NULL) return 1; // Erro ao abrir ficheiro
            
            sem_wait(sem[i]);
            
            int n;
            for (j = 0; j < 200; j++) {
                fscanf(in, "%d ", &n);
                fprintf(out, "%d\n", n);
            }

            if (i < NUM_CHILDS - 1) sem_post(sem[i+1]);

            fclose(in);
            fclose(out);
            exit(1);
        }
    }

    for (i = 0; i < NUM_CHILDS; i++)
        wait(NULL);
    
    if ((out = fopen(OUT_FILENAME, "r")) == NULL) return 1; // Erro ao abrir ficheiro
    
    char c;
    c = fgetc(out);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(out);
    }

    fclose(out);

    for (i = 0; i < NUM_CHILDS; i++) {
        if (sem_unlink(sem_name[i]) < 0) {
            perror("Error in sem_unlink()");
            exit(1);
        }
    }

   return 0; 
}

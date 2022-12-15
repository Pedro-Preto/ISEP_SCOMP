#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SIZE 10

int main() {
    // Criar pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    int i, j, n = 1;
    int buffer[SIZE];
    pid_t p = fork();
    if (p < 0) return 1; // Erro no fork
    if (p > 0) {
        close(fd[0]);

        for (i = 0; i < 3; i++) {
            for (j = 0; j < SIZE; j++) {
                buffer[j] = n;
                n++;
            }
            write(fd[1], &buffer, sizeof(int) * SIZE);
        }

        wait(NULL);

        close(fd[1]);
    } else {
        close(fd[1]);

        for (i = 0; i < 3; i++) {
            read(fd[0], &buffer, sizeof(int) * SIZE);
            for (j = 0; j < BUFFER_SIZE; j++)
                printf("%d\n", buffer[j]);
        }

        close(fd[0]);
        exit(0);
    }
    return 0;
}

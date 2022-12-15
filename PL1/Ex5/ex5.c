#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

 const int NR_OF_CHILDREN = 2;
    pid_t pids[NR_OF_CHILDREN];

    int i;
    for (i = 0; i < NR_OF_CHILDREN; i++) {
        pids[i] = fork();

        if (pids[i] == -1) {
            perror("Fork failed!");
            exit(-1);
        }

        if (pids[i] == 0) {
            int timeToWait = i + 1;//1 segundo no 1º processo filho, 2 segundos no 2º processo filho

            printf("Filho #%d criado com o PID = %d (tempo de espera de %d segundos)\n", i, getpid(), timeToWait);

            sleep(timeToWait);
            exit(timeToWait);
        }
    }

    pid_t p;
    int status;
    for (i = 0; i < NR_OF_CHILDREN; i++) {
        p = waitpid(pids[i], &status, 0);//1º argumento-pid, 2º argumento-status do pid
        if (WIFEXITED(status)) {//WIFEXITED-permite	verificar	se	o	filho	terminou	normalmente	
            printf("O filho #%d com o PID = %d terminou com valor = %d\n", i, p, WEXITSTATUS(status));//valor de saída do processo (1 byte)	passado	como parâmetro à função exit
        }
    }

    return 0;
}


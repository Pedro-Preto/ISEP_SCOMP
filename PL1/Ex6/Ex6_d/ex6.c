#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

    int i, status, exitStatus;
    pid_t p[4];

    for (i = 0; i < 4; i++){

        if ((p[i] = fork()) == 0){

            sleep(1);
            printf("Filho: %d.\n", i + 1);
            exit(i + 1); // terminou o processo e retornou i+1
        }
    }

    for (i = 0; i < 4; i++){

        if (p[i] % 2 == 0){

            waitpid(p[i], &status, 0);

            if (WIFEXITED(status)){

                exitStatus = WEXITSTATUS(status);
                printf("Filho com PID %d terminou com o status: %d\n", p[i], exitStatus);
            }
        }
    }
    printf("This is the end.\n");
}
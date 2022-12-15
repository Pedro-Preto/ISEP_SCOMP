#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    int i;
    int status;

    for (i = 0; i < 4; i++) {

        pid_t p1 = fork();

        if (p1 == 0) {
            sleep(1); /*sleep(): unistd.h*/
            printf("Filho: %d\n", i+1);
            exit(0);
        } else {
            if(p1 % 2 == 0){
                waitpid(p1, &status, 0);
            }
        }
    }

    printf("This is the end.\n");
}

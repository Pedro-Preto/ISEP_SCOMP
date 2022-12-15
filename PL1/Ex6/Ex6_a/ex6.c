#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() { 
    int i; 
    //int status; 
        for (i = 0; i < 4; i++) { 
            if (fork() == 0) { 
                sleep(1); /*sleep(): unistd.h*/ 
            } 
        } 
    printf("This is the end.\n");
    return 0;
} 

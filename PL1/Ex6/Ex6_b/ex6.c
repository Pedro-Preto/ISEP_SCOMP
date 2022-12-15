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
               printf("I am a Child\n");
               sleep(1); /*sleep(): unistd.h*/
               exit(i); 
            } 
        } 
    printf("This is the end.\n");
    return 0;
} 

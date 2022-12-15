#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 1000 

 int main () { 
 
 int numbers[ARRAY_SIZE]; /* array to lookup */ 
 int n; /* the number to find */ 
 time_t t; /* needed to init. the random number generator (RNG) 
*/ 
 int i; 

 /* intializes RNG (srand():stdlib.h; time(): time.h) */ 
 srand ((unsigned) time (&t)); 

 /* initialize array with random numbers (rand(): stdlib.h) */ 
 for (i = 0; i < ARRAY_SIZE; i++) 
 numbers[i] = rand () % 10000; 

 /* initialize n */ 
n = rand () % 10000; 

pid_t p=fork();
int count=0;
int arrayHalf=ARRAY_SIZE/2;
if(p==0){
    for(int i=0;i<arrayHalf;i++){
        if(numbers[i]==n){
            count++;
        }
    }
    exit(count);
}else{
    for(int j=arrayHalf;j<ARRAY_SIZE;j++){
        if(numbers[j]==n){
            count++;
        }
    }
}
int status;
    wait(&status);

        printf("O n (%d) foi encontrado %d vezes\n", n, count + WEXITSTATUS(status));

return 0;
} 

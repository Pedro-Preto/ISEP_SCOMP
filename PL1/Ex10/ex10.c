#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 2000 

 int main () { 
 
 int numbers[ARRAY_SIZE]; /* array to lookup */ 
 int n; /* the number to find */ 
 time_t t; /* needed to init. the random number generator (RNG) 
*/ 
 int i; 

 /* intializes RNG (srand():stdlib.h; time(): time.h) */ 
 srand ((unsigned) time (&t)); 

 /* initialize array with random numbers (rand(): stdlib.h) */ 
 for (i = 0; i < ARRAY_SIZE; i++) {
         numbers[i]=rand () % 200;;
 }
 /* initialize n */ 
n = rand () % 200; 
printf("Numero procurado- %d\n",n);

pid_t p[10];

   for(int i=0;i<10;i++){
      p[i]=fork();
      if(p[i]==0){
         int j=0;
         for(int j=0;j<ARRAY_SIZE;j++){
            if(numbers[j]==n){
               printf("Numero- %d e Posição- %d\n",numbers[j],j);
               exit(j);
            }
         }
         exit(255);
      }
   }
   
   int status;
   for (i = 0; i < 10; i++) {
      waitpid(p[i], &status, 0);
      if (WIFEXITED(status)) {
         printf("Index: %d\n",WEXITSTATUS(status));
        }
    }


return 0;
} 
  
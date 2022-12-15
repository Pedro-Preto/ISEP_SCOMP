#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 1000 
#define CHILDREN 5
 int main () { 
 
 int numbers[ARRAY_SIZE]; /* array to lookup */ 
 //int n; /* the number to find */ 
 time_t t; /* needed to init. the random number generator (RNG) 
*/ 
 int i; 

 /* intializes RNG (srand():stdlib.h; time(): time.h) */ 
 srand ((unsigned) time (&t)); 

 /* initialize array with random numbers (rand(): stdlib.h) */ 
 for (i = 0; i < ARRAY_SIZE; i++) {
         numbers[i]=rand () % 250;;
 }
 /* initialize n */ 
/*n = rand () % 250; 
printf("Numero procurado- %d\n",n);*/

 /*for (i = 0; i < ARRAY_SIZE; i++) {
   printf("%d\n",numbers[i]);

}*/

pid_t p[CHILDREN];

   for(int i=0;i<CHILDREN;i++){
      int maxValue=0;

      int startLimit= ARRAY_SIZE *i/5;
      printf("First Limit:%d\n",startLimit);

      int endLimit=(int) ARRAY_SIZE *(i+1)/5;
      printf("Last Limit:%d\n\n",endLimit);

      p[i]=fork();
      if(p[i]==0){
         for(int j=startLimit;j<endLimit;j++){
            if(numbers[j]>maxValue){   
               maxValue=numbers[j];
            }
         }
       
         exit(maxValue);
      }
   }

   int status;
   int foundNumber=-1; 
   for (i = 0; i < CHILDREN; i++) {
      waitpid(p[i], &status, 0);
      if (WIFEXITED(status)) {
         printf("Maior Processo nº:%d ,numero: %d\n",i+1,WEXITSTATUS(status));
         if(WEXITSTATUS(status)>foundNumber){
            foundNumber=WEXITSTATUS(status);
         }
      }
   }
   printf("Greater Number:%d\n\n",foundNumber);
    
    int result[ARRAY_SIZE];
    if (fork() == 0)
    {
        for (i = 0; i < 500; i++)
        {
            result[i]= (int)((numbers[i]*100) / foundNumber);
            printf("%d, %d\n", i, result[i]);
        }
    }
    else
   {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            for (i = 500; i < 1000; i++)
            {
                result[i] = (int)((numbers[i]*100) / foundNumber);
               printf("%d, %d\n", i, result[i]);
            }
        }
    }


return 0;
} 
  
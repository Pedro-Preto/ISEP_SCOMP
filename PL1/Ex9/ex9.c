#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


 int main () { 
 
pid_t p[10];

for(int i=0;i<10;i++){
   p[i]=fork();
   if(p[i]==0){
      int start=i*100+1;
      int end=start-1+100;
   //printf("Inicio-%d e Fim-%d\n",start,end);   
   
   while(start<end){
      printf("%d\n",start);
      start++;
   }
exit(0);
   }
}

   for(int i=0;i<10;i++){
      waitpid(p[i],NULL,0);
   }
      printf("I am the father and last to finish\n");

return 0;
} 

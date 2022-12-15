#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    int childCredit = 20;

    int up[2];//filho para o pai 
    int down[2];//pai para o filho

    pid_t p;

   if(pipe(up)== -1)
    {
        perror("Pipe failed");
        return 1;
    }
      
    if(pipe(down)== -1)
    {
        perror("Pipe failed");
        return 1;
    }

    p=fork();
    
    if (p > 0)
    { 
        //Garante que as bets nem sempre são iguais
    	srand((unsigned) time(NULL) * getpid()); 

        close(up[ESCRITA]);
        close(down[LEITURA]);
        
        while(childCredit>0){

           int randonBetFather =1+rand() % 5 ; //gera número aleatório de 1 a 5

           int num;
           if(childCredit>0){
               num=1;
            }else{
                num=0;
           }

          write(down[ESCRITA],&num,sizeof(num));           
        
          int childBet;
          read(up[LEITURA],&childBet ,sizeof(childBet));
          
          printf("Aposta Filho: %d; Resultado Pai: %d\n", childBet,randonBetFather);

          if(randonBetFather==childBet){
            childCredit+=10;
          }else{
            childCredit-=5;
          }
        
         write(down[ESCRITA],&childCredit,sizeof(childCredit));           
        
        } 
        close(down[ESCRITA]);
        close(up[LEITURA]);
        
    
}
    if (p ==0)
    {
        //Garante que as bets nem sempre são iguais
        srand((unsigned) time(NULL) * getpid()); 

        close(up[LEITURA]);
        close(down[ESCRITA]);

        while(childCredit>0){
            int num;
            read(down[LEITURA], &num,sizeof(num));
           
            if(num==1){
                int randonBetChild = 1+ rand() % 5; //gera número aleatório de 1 a 5
                write(up[ESCRITA],&randonBetChild,sizeof(randonBetChild));           
             
                childCredit=0;
                read(down[LEITURA], &childCredit,sizeof(childCredit));
                
                printf("Current Credit:%d\n",childCredit);
        

            }else if(num==0){
                close(up[ESCRITA]);
                close(down[LEITURA]);
                exit(EXIT_SUCCESS);
            }
        
        
        }
  }


    return 0;
}





  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main(){

   int fd[2];
    const int LEITURA = 0;
    const int ESCRITA = 1; 
    char WORD1[11]="Hello World";
    char WORD2[7]="Goodbye";

    /* cria o pipe */ 
 if(pipe(fd) == -1){
      perror("Pipe failed"); 
      return 1; 
    }
pid_t p=fork();

if(p>0){
    
    /* fecha a extremidade de leitura */ 
    close(fd[LEITURA]);

    write(fd[ESCRITA],&WORD1,sizeof(WORD1)); 
    write(fd[ESCRITA],&WORD2,sizeof(WORD2)+1); 

    /* fecha a extremidade de escrita */ 
    close(fd[ESCRITA]); 
}else{
        printf("Filho criado com o PID = %d\n", getpid());

        /* fecha a extremidade de escrita */ 
        close(fd[ESCRITA]);
        char word1[11];
        char word2[7];

        read(fd[LEITURA], &word1, sizeof(word1));
        read(fd[LEITURA], &word2, sizeof(word2));
        
        printf("First Message: %s\n", word1);
        printf("Second Message: %s\n", word2);
        
        /* fecha a extremidade de leitura */ 
        close(fd[LEITURA]);
        exit(EXIT_SUCCESS);
    }
    // Wait for the process to finish and grap the status
    int status;
    pid_t pidExit = wait(&status);
    printf("O processo com o PID = %d terminou com valor = %d\n", pidExit,   WEXITSTATUS(status));

    exit(EXIT_SUCCESS);

return 0;
}




  
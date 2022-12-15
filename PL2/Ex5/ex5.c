#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main(){

    int up[2];
    int down[2];

    const int LEITURA = 0;
    const int ESCRITA = 1; 
    const char WORD[10]="lower case";

    /* cria o pipe */ 
 if(pipe(up) == -1){
      perror("Pipe failed"); 
      return 1; 
    }
/* cria o pipe */ 
 if(pipe(down) == -1){
      perror("Pipe failed"); 
      return 1; 
    }

pid_t p=fork();

if(p>0){
    
    /* fecha a extremidade de leitura */ 
    close(down[LEITURA]);
    close(up[ESCRITA]);
    char word[10];
    read(up[LEITURA], &word, sizeof(word));
    
    /*To UPPERCASE*/
    int i=0;
while( word[i] != '\0' ) 
     {
        // if character is in lowercase
        // then subtract 32 
        if( word[i] >= 'a' && word[i] <= 'z' )
        {
           word[i] = word[i] - 32;
        }

        // increase iterator variable
        i++;
     }
    write(down[ESCRITA],&word,sizeof(word)); 

    /* fecha a extremidade de escrita */ 
    close(down[ESCRITA]);
    close(up[LEITURA]);
    wait(NULL);
}else{
        /* fecha a extremidade de escrita */ 
        close(down[ESCRITA]);
        close(up[LEITURA]);

        write(up[ESCRITA],&WORD,sizeof(WORD)); 
        char word[10];
        read(down[LEITURA], &word, sizeof(word));
        printf("Upper Case:%s\n",word);
        /* fecha a extremidade de leitura */ 
        close(down[LEITURA]);
        close(up[ESCRITA]);
    
        exit(EXIT_SUCCESS);
    }
    // Wait for the process to finish and grap the status
  
    exit(EXIT_SUCCESS);

return 0;
}




  
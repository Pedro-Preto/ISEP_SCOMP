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
    const int BUFFER_SIZE =  15;
    FILE* ptr;
    char ch;

    /* cria o pipe */ 
 if(pipe(fd) == -1){
      perror("Pipe failed"); 
      return 1; 
    }
pid_t p=fork();

if(p>0){
    
    /* fecha a extremidade de leitura */ 
    close(fd[LEITURA]);
    ptr = fopen("ex4.txt", "r");
  if (NULL == ptr) {
        printf("file can't be opened \n");
    }
        do {
        ch = fgetc(ptr);
        write(fd[ESCRITA],&ch,sizeof(ch)); 
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);

    /* fecha a extremidade de escrita */ 
    close(fd[ESCRITA]); 
    wait(NULL);
}else{
        /* fecha a extremidade de escrita */ 
        close(fd[ESCRITA]);
        char word[BUFFER_SIZE];
       while(read(fd[LEITURA], &word, sizeof(word))) {
        printf("%s\n", word);
       }
        /* fecha a extremidade de leitura */ 
        close(fd[LEITURA]);
        exit(EXIT_SUCCESS);
    }
    // Wait for the process to finish and grap the status
  
    exit(EXIT_SUCCESS);

return 0;
}




  
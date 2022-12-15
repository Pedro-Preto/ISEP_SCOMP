/*
 *  SCOMP: 2021/2022
 *      Author: Ana Barreiro (1190353) & Pedro Preto (1190963)
 */

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
    int number;
    char word[20];

    /* cria o pipe */ 
 if(pipe(fd) == -1){
      perror("Pipe failed"); 
      return 1; 
    }
pid_t p=fork();

if(p>0){
    
    
    /* fecha a extremidade de leitura */ 
    close(fd[LEITURA]);

    printf("Number: \n");
    scanf("%d", &number);

    printf("Word: \n");
    scanf("%s", word);

    write(fd[ESCRITA],&number,sizeof(int)); 
    write(fd[ESCRITA],&word,sizeof(word)); 

    /* fecha a extremidade de escrita */ 
    close(fd[ESCRITA]); 
}else{
        /* fecha a extremidade de escrita */ 
        close(fd[ESCRITA]);
        int number=0;
        char word[20]={};
        read(fd[LEITURA], &number, sizeof(number));
        read(fd[LEITURA], &word, sizeof(word));

        /* fecha a extremidade de leitura */ 
        close(fd[LEITURA]);
        printf("Number: %d\n", number);
        printf("Word: %s\n",word);
        exit(EXIT_SUCCESS);

    }

return 0;
}




  
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

struct S1 {
    int n;
    char w[20];
};

if(p>0){
    struct S1 s;

    /* fecha a extremidade de leitura */ 
    close(fd[LEITURA]);

    printf("Number: \n");
    scanf("%d", &number);

    printf("Word: \n");
    scanf("%s", word);

    s.n=number;
    strcpy(s.w, word);

    write(fd[ESCRITA],&s,sizeof(s)); 

    /* fecha a extremidade de escrita */ 
    close(fd[ESCRITA]); 
}else{
        /* fecha a extremidade de escrita */ 
        close(fd[ESCRITA]);
        struct S1 s;

        read(fd[LEITURA], &s, sizeof(s));

        /* fecha a extremidade de leitura */ 
        close(fd[LEITURA]);
        printf("Number: %d\n", s.n);
        printf("Word: %s\n",s.w);
        exit(EXIT_SUCCESS);

    }

return 0;
}




  
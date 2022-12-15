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
    
    /* cria o pipe */ 
 if(pipe(fd) == -1){
      perror("Pipe failed"); 
      return 1; 
    }
pid_t p=fork();

if(p>0){
    
    printf("PID do Pai: %d\n", getpid());
    
    /* fecha a extremidade de leitura */ 
    close(fd[LEITURA]);
    pid_t p=getpid();
    write(fd[ESCRITA],&p,sizeof(p)); 
    /* fecha a extremidade de escrita */ 
    close(fd[ESCRITA]); 
}else{
        /* fecha a extremidade de escrita */ 
        close(fd[ESCRITA]);
        pid_t pid_parent=0;
        read(fd[LEITURA], &pid_parent, sizeof(pid_parent));
        /* fecha a extremidade de leitura */ 
        close(fd[LEITURA]);
        printf("PID do Pai lido pelo Filho: %d\n", pid_parent);
    }

return 0;
}




  
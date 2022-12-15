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
    const int CHILDREN= 5;
    const int PIPES= 6;

    int fd[PIPES][2];

    pid_t p[CHILDREN];

for(int i=0;i<CHILDREN;i++){
   if(pipe(fd[i])== -1)
    {
        perror("Pipe failed");
        return 1;
    }

}
int randomFather=rand() % 500 + 1;
printf("Pai ,PID: %d = %d\n",getpid(),randomFather);

for(int i=0;i<CHILDREN;i++){
   int numRandomFilho=rand() % 500 + 1;
    p[i]=fork();
    if(p[i]>0 && i==0){
    
    close(fd[i][LEITURA]);
    write(fd[i][ESCRITA], &randomFather, sizeof(randomFather));
	close(fd[i][ESCRITA]);
    }
   
    if (p[i] ==0)
    {
       printf("Filho:%d ,PID: %d = %d\n", i+1,getpid(), numRandomFilho);
        int numLido;
        close(fd[i][ESCRITA]);
        close(fd[i+1][LEITURA]);

		read(fd[i][LEITURA], &numLido, sizeof(numLido));
		
        int greater;
        if(numRandomFilho>numLido){
            greater=numRandomFilho;
        }else{
            greater=numLido;
        }
        
        
		write(fd[i+1][ESCRITA], &greater, sizeof(greater));

        close(fd[i][LEITURA]);
		close(fd[i+1][ESCRITA]);
		
        exit(EXIT_SUCCESS);
    }
}

    
    close(fd[5][ESCRITA]);
    wait(NULL);
    int n;
    read(fd[5][LEITURA], &n, sizeof(n));
    close(fd[5][LEITURA]);

    printf("Maior Numero = %d\n", n);


    return 0;
}








  
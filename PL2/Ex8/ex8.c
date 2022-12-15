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
    const char WORD[4]="Win";
    int fd[2];

    pid_t p[10];

       struct Game {
        char win[4];
        int number;
    };

   if(pipe(fd)== -1)
    {
        perror("Pipe failed");
        return 1;
    }


for(int i=0;i<10;i++){
    p[i]=fork();
    if (p[i] > 0)
    { 
        close(fd[LEITURA]);
        
        struct Game g;
        g.number=i+1;
        strcpy(g.win, WORD);
     //   printf("ROUND->%d\n",g.number);
        write(fd[ESCRITA], &g, sizeof(g));

        close(fd[ESCRITA]);
        sleep(2);
    }
    if (p[i] ==0)
    {
        close(fd[ESCRITA]);
        struct Game g;

        read(fd[LEITURA], &g, sizeof(g));
        printf("Message:%s | Round:%d\n",g.win,g.number);

        close(fd[LEITURA]);
        exit(i+1);
    }
}
    printf("\n\n");    

    pid_t p_temp;
    int status;
    for (int i = 0; i < 10; i++) {
        p_temp = waitpid(p[i], &status, 0);
        if (WIFEXITED(status)) {
            printf("PID = %d --> Round:%d\n", p_temp,i+1);
        }
    }

    return 0;
}





  
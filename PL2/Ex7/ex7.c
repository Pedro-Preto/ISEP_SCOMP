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

    int vec1[1000];
    int vec2[1000];
    time_t t; //for srand
    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    int i;
    for (i = 0; i < 1000; i++)
    {
        vec1[i] = rand() % (256);
    }
    for (i = 0; i < 1000; i++)
    {
        vec2[i] = rand() % (256);
    }

    int fd[5][2];

    pid_t p[5];

    

for(int i=0;i<5;i++){
   if(pipe(fd[i])== -1)
    {
        perror("Pipe failed");
        return 1;
    }

}
int final=0;
for(int i=0;i<5;i++){
    p[i]=fork();
    if (p[i] > 0)
    {
        close(fd[i][ESCRITA]);
        int tmp;
      
        read(fd[i][LEITURA], &tmp, 4);
        final += tmp;
        
        close(fd[i][LEITURA]);

        
    }
    
    if (p[i] ==0)
    {
        close(fd[i][LEITURA]);
        int tmp = 0;
        int start = i * 200;
        int finish = start + 200;
        for (int j = start; j < finish; j++)
        {
            tmp +=  + vec1[j] + vec2[j];
        }
        write(fd[i][ESCRITA], &tmp, 4);
        close(fd[i][ESCRITA]);
        exit(EXIT_SUCCESS);
    }
}
    wait(NULL);
    printf("Final result is %d\n", final);

    exit(EXIT_SUCCESS);

    return 0;
}





  
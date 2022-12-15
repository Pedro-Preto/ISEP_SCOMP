#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

typedef struct {
	int customer_code;
	int product_code;
	int quantity;
} sale;

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int CHILDREN= 10;

    sale sales[50000];
	int products[50000];
	
	//preencher sales com números aleatórios
	for(int i = 0; i < 50000; i++) {
		sales[i].customer_code = rand() % 500;
		sales[i].product_code = rand() % 1000;
		sales[i].quantity = rand() % 22;
	}
 
    int fd[5][2];

    pid_t p[CHILDREN];

for(int i=0;i<CHILDREN;i++){
   if(pipe(fd[i])== -1)
    {
        perror("Pipe failed");
        return 1;
    }

}
int position=0;
for(int i=0;i<CHILDREN;i++){
    p[i]=fork();
    if (p[i] > 0)
    { 
        close(fd[i][ESCRITA]);
        int product_code;
       while( (read(fd[i][LEITURA], &product_code, sizeof(product_code)))>0){
            products[position]=product_code;
            position++;
       }

        close(fd[i][LEITURA]);
      
    }
    if (p[i] ==0)
    {
        close(fd[i][LEITURA]);
        int minLim = 5000 * i;
		int maxLim = 5000 * i + 5000; 

        for(int j=minLim;j<maxLim;j++){
            if(sales[j].quantity>20){
               // printf("Position-%d |Quantity-%d| Product-%d\n",j,sales[j].quantity,sales[j].product_code); 
                write(fd[i][ESCRITA], &sales[j].product_code,sizeof(sales[j].product_code));           
            }
        }
        close(fd[i][ESCRITA]);
        exit(EXIT_SUCCESS);

    }
}
    printf("\n\n");    
 
    for (int i = 0; i < position; i++) {
      	printf("Position-%d | Product-%d\n",i, products[i]); 

    }

    return 0;
}





  
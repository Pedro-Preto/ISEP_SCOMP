
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void menu();

int main() {    
menu();
return 0;
} 

void menu(){
	int option;
	do{
	printf("\n\nEscolha uma opção\n");
	printf("1-Stop\n");
	printf("2-Continuar\n");
	printf("3-Terminar\n");
	scanf("%d",&option);
	
	switch(option){
	case 1:
	printf("Insira o pid do processo\n");
	int pid1;
	scanf("%d",&pid1);
	kill(pid1,SIGSTOP);
	 break;
	case 2:
	printf("Insira o pid do processo\n");
	int pid2;
	scanf("%d",&pid2);
	kill(pid2,SIGCONT);
	 break;
	case 3:
	printf("Insira o pid do processo\n");
	int pid3;
	scanf("%d",&pid3);
	kill(pid3,SIGKILL);
	break;
	
	}	
	}while(option!=0);
}



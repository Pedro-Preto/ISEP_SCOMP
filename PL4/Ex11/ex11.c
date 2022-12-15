#include<stdio.h>
#include<time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>

#define NUM_PORTAS 3 /* número de processos filho */
#define NUM_PASSENGERS 200 /* número máximo de passageiros */
#define SEMAFORO_PORTA_01 0
#define SEMAFORO_PORTA_02 1
#define SEMAFORO_PORTA_03 2
#define SEMAFORO_CAPACIDADE 3
#define QUANTIDADE_SEMAFOROS 4

int main(){
    int i, j, entered, left, capacityLeft;

	const char SEMAFOROS_NAME[QUANTIDADE_SEMAFOROS][80] = {"SEM_01", "SEM_02","SEM_03","SEM_04"};
    const int SEMAFOROS_INITIAL_VALUE[QUANTIDADE_SEMAFOROS] = {1, 1, 1, 200};

   sem_t *semaforos[QUANTIDADE_SEMAFOROS] ;

      // Create semaphore
    for(int i=0;i<QUANTIDADE_SEMAFOROS;i++){
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }
	

    for(i = 0; i < NUM_PASSENGERS; i++) { /* para cada passageiro */
		pid_t pid = fork();
		
		if(pid == -1) {
			perror("Failed fork\n");
			exit(0);

		} else if(pid == 0) {
			
			/* simulação de entrada no comboio */
			
			entered = 0; /* variável que indica se alguém já entrou no comboio */
			while(entered == 0) { /* enquanto ninguém entrou */
				for(j = 0; j < NUM_PORTAS; j++) { /* para cada porta */
				
					if(sem_trywait(semaforos[SEMAFORO_PORTA_01]) != 0) { /* avançar se porta 1 não estiver bloqueada */
						
						sem_wait(semaforos[SEMAFORO_CAPACIDADE]); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro entrou pela porta 1 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_01]); /* desbloquear porta 1 */
						break;
					}
					
					if(sem_trywait(semaforos[SEMAFORO_PORTA_02]) != 0) { /* avançar se porta 2 não estiver bloqueada */
						
						sem_wait(semaforos[SEMAFORO_CAPACIDADE]); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro entrou pela porta 2 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_02]); /* desbloquear porta 2 */
						break;
					}

					if(sem_trywait(semaforos[SEMAFORO_PORTA_03]) != 0) { /* avançar se porta 3 não estiver bloqueada */
						
						sem_wait(semaforos[SEMAFORO_CAPACIDADE]); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro entrou pela porta 3 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_03]); /* desbloquear porta 3 */
						break;
					}
				}
			}
			
			/* simulação de saída do comboio */
			
			left = 0; /* variável que indica se alguém já saiu do comboio */
			while(left == 0) { /* enquanto ninguém saiu */
				for(j = 0; j < NUM_PORTAS; j++) { /* para cada porta */
				
					if(sem_trywait(semaforos[SEMAFORO_PORTA_01]) != 0) { /* avançar se porta 1 não estiver bloqueada */
											
						sem_post(semaforos[SEMAFORO_CAPACIDADE]); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro saiu pela porta 1 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_01]); /* desbloquear porta 1 */
						break;
					}
					
					if(sem_trywait(semaforos[SEMAFORO_PORTA_02]) != 0) { /* avançar se porta 2 não estiver bloqueada */
											
						sem_post(semaforos[SEMAFORO_CAPACIDADE]); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro saiu pela porta 2 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_02]); /* desbloquear porta 2 */
						break;
					}
					
					if(sem_trywait(semaforos[SEMAFORO_PORTA_03]) != 0) { /* avançar se porta 3 não estiver bloqueada */
											
						sem_post(semaforos[SEMAFORO_CAPACIDADE]); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(semaforos[SEMAFORO_CAPACIDADE], &capacityLeft);
						printf("Passageiro saiu pela porta 3 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(semaforos[SEMAFORO_PORTA_03]); /* desbloquear porta 3 */
						break;
					}
				}
			}
			exit(0);
		}
	}
	
	/* processo pai espera que processos filho acabem */
	for(i = 0; i < NUM_PASSENGERS; i++){
		wait(NULL);
	}
	/* fechar os semáforos */
		for (int i = 0; i < QUANTIDADE_SEMAFOROS; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Cicl para remover todos os semaforos do SO
	for (int i = 0; i < QUANTIDADE_SEMAFOROS; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}


    return 0;

}
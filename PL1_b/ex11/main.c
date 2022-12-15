#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct {
    char cmd[32];
    int tempo;
} comando;

volatile sig_atomic_t exec_ended = 0;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    exec_ended = 1;
}

int main() {
    // Definir handler do signal  (SIGCHLD)
    // Vai ser tratado o SIGCHLD, que é recebido após um child terminar
    // Desta forma é possível saber que a execução do comando terminou
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGCHLD, &act, NULL);

    // Definir a array com os comandos
    // IMPORTANTE: é necessário fazer *make* dos 3 programas de teste antes de executar este
    comando execs[3];
    strcpy(execs[0].cmd, "./programs/prog_a/exe");
    execs[0].tempo = 1; // Tempo < que a execução (deve mostrar mensagem)
    strcpy(execs[1].cmd, "./programs/prog_b/exe");
    execs[1].tempo = 5;
    strcpy(execs[2].cmd, "./programs/prog_c/exe");
    execs[2].tempo = 6;
    int arr_size = 3;

    // Percorrer a array e executar programas
    // O processo filho é responsável por executar o programa
    // O processo pai regula o tempo limite de execução
    pid_t p;
    int i;
    for (i = 0; i < arr_size; i++) {
        p = fork();
        if (p < 0) exit(1);
        if (p == 0) {
            char *args[] = {execs[i].cmd, NULL};
            execvp(args[0], args); // Função exec substitui o child pelo executável
            exit(1);
        } else {
            int tempo = 0;
            while (!exec_ended && tempo < execs[i].tempo) {
                sleep(1);
                if (!exec_ended) tempo++;
            }
            if (tempo >= execs[i].tempo) { // Se tiver ultrapassado o tempo máximo de execução, mata o child e mostra mensagem
                kill(p, SIGTERM);
                printf("The command %s didn't end in its allowed time!\n", execs[i].cmd);
            }
            wait(NULL);
            exec_ended = 0;
        }
    }
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    sigset_t set;
    sigprocmask(SIG_BLOCK, NULL, &set); //change the signal mask of the calling thread
    
    write(STDOUT_FILENO, "Sinais bloquados:\n", 18);
    
    int i;
    for (i = 1; i <= 64; i++) { // Percorrer todos os sinais para saber quais estão bloqueados
        if (sigismember(&set, i)) {
            //printf("%d;", i);
            char sig[4];
            sprintf(sig, "%d;", i);
            write(STDOUT_FILENO, sig, strlen(sig));
        }
    }
   
    write(STDOUT_FILENO, "\n", 1);
}

int main() {
   
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask); // block all the signals
   
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
   
    sigaction(SIGUSR1, &act, NULL);

    raise(SIGUSR1); // ANSIC C function - signal SIGUSR1 is raidsed
    //The raise() function sends the signal, sig, to the process that issued the raise().
    // If the signal is not blocked, it is delivered to the sender before raise() returns.
    return 0;
}


/* Os seguintes sinais não foram bloqueados:
9 - SIGKILL
19 - SIGSTOP

Estes sinais sinais não podem ser handled, bloqueados nem ignorados, já que apenas são handled pelo Kernel para que haja sempre uma forma de sair do programa/processo.*/
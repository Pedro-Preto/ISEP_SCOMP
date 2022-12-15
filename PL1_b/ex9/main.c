#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t task_executed = 0; //é usado para sinais
//possível ser usado assincrunamente


void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    task_executed = 1;
}

int main() {
    
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    
    sigaction(SIGUSR1, &act, NULL);

    pid_t p = fork();
    if (p < 0) exit(1);
    if (p > 0) {
        sleep(3);
        printf("Task A - 3 seconds passed;\n");
        kill(p, SIGUSR1);
    
    } else {
    
        srand(time(NULL));
        
        int duration = rand() % 5 + 1;
        sleep(duration);
        printf("Task B - %d seconds passed;\n", duration);
        
        if (!task_executed)
        pause();

        printf("Task C - executed.\n");
        exit(0);
    
    }

    wait(NULL);

    return 0;
}
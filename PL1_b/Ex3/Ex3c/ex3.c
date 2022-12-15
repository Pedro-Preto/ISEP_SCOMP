#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t signalPid = -1;

void handle_signal(int signo, siginfo_t * info, void * cont){
    //signalPID = getpid(); //which process sent the signal
    signalPid = info -> si_pid;  
    // si_pid envia o processo do outro terminal
}

int main(){

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handle_signal;
    
    sigaction(SIGUSR1, &act, NULL);
    /* SIGUSR1 will now be captured and handled (ANSI C). */
    //printf("process PID %d\n", getpid());
    pause(); // wait for the signal

    printf(" SIGUSR1 is now captured and sent by the process with PID %d\n", signalPid);
return 0;
}




  
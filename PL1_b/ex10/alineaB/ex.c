#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t str_read = 0;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    if (!str_read) {
        write(STDOUT_FILENO, "\n\nTo slow, that is why the program will end!\n", 45);
        exit(0); // Uso de _exit(), pois é mais seguro que exit() em um signal handler
    }
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    char str[50];
    printf("String: ");
    pid_t p = fork();
    if (p == 0) {
        sleep(10);
        kill(getppid(), SIGUSR1);
    } else {
        scanf("%s", str);
        str_read = 1;
        int len = strlen(str);
        printf("String length = %d\n", len);
        sleep(20);
        printf("Successful execution!\n");
    }
	return 0;
}

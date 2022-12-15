#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t str_read = 0;

void handle_signal(int signo, siginfo_t *sinfo, void *context) {
    if (!str_read) {
        write(STDOUT_FILENO, "\n\nTo slow, that is why the program will end!\n", 45);
        exit(0);
    }
}

int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM, &act, NULL);

    char str[50];
    printf("String: ");
    alarm(10);
    scanf("%s", str);
    str_read = 1;
    alarm(0); // Cancela alarm anteriormente definido
    int len = strlen(str);
    printf("String length = %d\n", len);
    sleep(20);
    printf("Successful execution!\n");
	return 0;
}



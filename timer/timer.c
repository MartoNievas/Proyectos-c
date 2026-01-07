#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t countdown_seconds = 0;
volatile sig_atomic_t tick = 0;
volatile sig_atomic_t end_timer = 0;

/* Handler: NO imprime */
void signal_handler(int sig)
{
    (void)sig;
    tick = 1;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <seconds>\n", argv[0]);
        exit(1);
    }

    countdown_seconds = atoi(argv[1]);

    printf("Starting timer of %d seconds...\n", countdown_seconds);

    /* sigaction */
    struct sigaction act = {0};
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    /* timer */
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    /* loop principal */
    while (!end_timer) {
        pause();

        if (tick) {
            tick = 0;

            countdown_seconds--;

            printf("\r\033[KRemaining: %d seconds", countdown_seconds);
            fflush(stdout);

            if (countdown_seconds <= 0) {
                end_timer = 1;
            }
        }
    }

    printf("\nTimer elapsed!\n");
    return 0;
}

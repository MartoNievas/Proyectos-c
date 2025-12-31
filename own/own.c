#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pty.h>
#include <sys/select.h>
#include <sys/wait.h>

int main() {
    int master_fd;
    pid_t pid;

    pid = forkpty(&master_fd, NULL, NULL, NULL);
    if (pid < 0) {
        perror("forkpty");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execlp("bash", "bash", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        char buf[256];
        ssize_t n;
        fd_set rfds;

        while (1) {
            FD_ZERO(&rfds);
            FD_SET(master_fd, &rfds);
            FD_SET(STDIN_FILENO, &rfds);

            int ret = select(master_fd + 1, &rfds, NULL, NULL, NULL);
            if (ret < 0) break;

            // Leer del shell y mostrar
            if (FD_ISSET(master_fd, &rfds)) {
                n = read(master_fd, buf, sizeof(buf)-1);
                if (n <= 0) break;
                buf[n] = '\0';
                write(STDOUT_FILENO, buf, n);
            }

            // Leer del teclado y enviar al shell
            if (FD_ISSET(STDIN_FILENO, &rfds)) {
                n = read(STDIN_FILENO, buf, sizeof(buf)-1);
                if (n <= 0) break;
                write(master_fd, buf, n);
            }
        }

        waitpid(pid, NULL, 0);
    }

    return 0;
}


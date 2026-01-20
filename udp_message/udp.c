#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256

static void check_mode(int argc, char *argv[], bool *listen_mode,
                       bool *complete_mode, int *port) {
  if (argc == 3 && strcmp(argv[1], "-l") == 0) {
    *listen_mode = true;
    *port = atoi(argv[2]);
  } else if (argc == 5 && strcmp(argv[1], "-d") == 0 &&
             strcmp(argv[3], "-p") == 0) {
    *port = atoi(argv[4]);
    // ./ udp -d <device_ip> -p <port>
  } else if (argc == 5 && strcmp(argv[1], "-d") == 0 &&
             strcmp(argv[argc - 2], "-l") == 0) {
    *complete_mode = true;
    *port = atoi(argv[argc - 1]);
  } else {
    printf("Usage:\n");
    printf("  %s -l <port>\n", argv[0]);
    printf("  %s -d <device_ip> -p <port>\n", argv[0]);
    printf("  %s -d <device_ip> -l <port>\n", argv[0]);
    exit(1);
  }
}

int main(int argc, char *argv[]) {

  bool listen_mode = false;
  bool complete_mode = false;
  int port = 0;

  check_mode(argc, argv, &listen_mode, &complete_mode, &port);

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket");
    return 1;
  }

  /* ================= LISTEN MODE ================= */
  if (listen_mode) {
    struct sockaddr_in local_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    bzero(&local_addr, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
      perror("bind");
      close(sockfd);
      return 1;
    }

    printf("Listening on port %d...\n", port);

    for (;;) {
      int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                       (struct sockaddr *)&client_addr, &client_len);

      if (n < 0) {
        perror("recvfrom");
        continue;
      } else if (n == 0) {
        continue;
      }

      buffer[n] = '\0';

      if (buffer[n - 1] == '\n') {
        buffer[n - 1] = '\0';
      }

      printf("From %s:%d -> %s\n", inet_ntoa(client_addr.sin_addr),
             ntohs(client_addr.sin_port), buffer);
    }

    /* ================= COMPLETE MODE ================= */
  } else if (complete_mode) {

    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    fd_set readfds;
    char buffer[BUF_SIZE];
    char *ip = argv[2];
    socklen_t addr_len = sizeof(remote_addr);

    bzero(&local_addr, sizeof(local_addr));
    bzero(&remote_addr, sizeof(remote_addr));

    /*local addr */
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(port);

    /* remote addr */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);

    if (inet_aton(ip, &remote_addr.sin_addr) == 0) {
      printf("Invalid IP address\n");
      close(sockfd);
      return 1;
    }

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
      perror("bind");
      close(sockfd);
      return 1;
    }

    printf("Listening on port %d...\n", port);
    printf("Ready to communicate with %s:%d\n", ip, port);
    printf("Message: ");
    fflush(stdout);

    for (;;) {
      FD_ZERO(&readfds);
      FD_SET(sockfd, &readfds);
      FD_SET(STDIN_FILENO, &readfds);

      int maxfd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

      if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
        perror("select");
        break;
      }

      if (FD_ISSET(sockfd, &readfds)) {
        struct sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);

        int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                         (struct sockaddr *)&from_addr, &from_len);

        if (n > 0) {
          buffer[n] = '\0';

          if (buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
          }

          printf("\nFrom %s:%d -> %s\n", inet_ntoa(from_addr.sin_addr),
                 ntohs(from_addr.sin_port), buffer);
          printf("Message: ");
          fflush(stdout);
        } else if (n < 0) {
          perror("recvfrom");
        }
      }

      if (FD_ISSET(STDIN_FILENO, &readfds)) {
        if (!fgets(buffer, BUF_SIZE, stdin))
          break;

        int sent = sendto(sockfd, buffer, strlen(buffer), 0,
                          (struct sockaddr *)&remote_addr, sizeof(remote_addr));
        if (sent < 0) {
          perror("sendto");
        }
        printf("Message: ");
        fflush(stdout);
      }
    }
  }

  /* ================= SEND MODE ================= */
  else {
    const char *ip = argv[2];
    char message[BUF_SIZE];

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_aton(ip, &server_addr.sin_addr) == 0) {
      printf("Invalid IP address\n");
      close(sockfd);
      return 1;
    }

    printf("Sending to %s:%d\n", ip, port);

    while (true) {
      printf("Message: ");
      fflush(stdout);

      if (!fgets(message, sizeof(message), stdin))
        break;

      int sent = sendto(sockfd, message, strlen(message), 0,
                        (struct sockaddr *)&server_addr, sizeof(server_addr));

      if (sent < 0) {
        perror("ERROR in sendto\n");
      } else {
        printf("Sent %d bytes\n", sent);
      }
    }
  }

  close(sockfd);
  return 0;
}

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {

  bool listen_mode = false;
  int port = 0;

  if (argc == 3 && strcmp(argv[1], "-l") == 0) {
    listen_mode = true;
    port = atoi(argv[2]);
  } else if (argc == 5 && strcmp(argv[1], "-d") == 0 &&
             strcmp(argv[3], "-p") == 0) {
    port = atoi(argv[4]);
  } else {
    printf("Usage:\n");
    printf("  %s -l <port>\n", argv[0]);
    printf("  %s -d <device|ip> -p <port>\n", argv[0]);
    return 1;
  }

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

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
      perror("connect");
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

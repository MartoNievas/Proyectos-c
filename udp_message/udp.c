#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*Defines data*/
#define PORT (9999)
#define MAXLINE 1000
#define MARTIN_IP "192.168.1.198"
#define MOMI_IP "192.168.1.151"
#define MARCE_IP "0"
typedef enum {
  MARTIN,
  MOMI,
  MARCE,
} Devices_Kind;

typedef struct {
  Devices_Kind kind;
  const char *ip;
} Device;

const char *string_to_ip(const char *device) {
  if (!device) {
    printf("ERROR: Null pointer in string\n");
    return NULL;
  }
  if (strcmp(device, "martin") == 0) {
    return MARTIN_IP;
  } else if (strcmp(device, "momi") == 0) {
    return MOMI_IP;
  } else if (strcmp(device, "marce") == 0) {
    return MARCE_IP;
  }
  return NULL;
}

char buffer[100];
char message[256];
int main(int argc, char *argv[]) {

  printf("Welcome to the UDP message sender\n");
  if (argc != 2) {
    printf("Usage: %s <device or ip>\n", argv[0]);
    return 0;
  }

  const char *device = argv[1];
  const char *ip = string_to_ip(device);

  int sockfd;

  struct sockaddr_in servaddr;

  // clear servaddr
  bzero(&servaddr, sizeof(servaddr));

  if (!ip) {
    ip = device;
  }
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_family = AF_INET;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  // connect server
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("ERROR: Connect failed \n");
    exit(0);
  }

  for (;;) {
    printf("Send a message: \n");
    fflush(stdout);
    if (!fgets(message, sizeof(message), stdin))
      break;

    const char *source = message;
    sendto(sockfd, source, strlen(source), 0, (struct sockaddr *)NULL,
           sizeof(servaddr));
  }

  recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
  puts(buffer);
  close(sockfd);

  return 0;
}

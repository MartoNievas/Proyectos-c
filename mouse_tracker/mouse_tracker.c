#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h> /*need to mallco and printf*/
#include <string.h>
#include <time.h>
#include <unistd.h>
// event11 -> touchpad (Thinkpad T480)
// event12 -> red_tacker (Thinkpad T480)

#define MAX_X 1920
#define MAX_Y 1080

void get_code(struct input_event *event, char *code, int *x, int *y) {
  if (event->type == EV_REL) {
    switch (event->code) {
    case REL_X:
      strcpy(code, "REL_X");
      *x += event->value;
      if (*x < 0)
        *x = 0;
      if (*x > MAX_X)
        *x = MAX_X;
      break;
    case REL_Y:
      strcpy(code, "REL_Y");
      *y += event->value;
      if (*y < 0)
        *y = 0;
      if (*y > MAX_Y)
        *y = MAX_Y;
      break;
    default:
      strcpy(code, "REL_OTHER");
      break;
    }
  } else if (event->type == EV_ABS) {
    switch (event->code) {
    case ABS_X:
      strcpy(code, "ABS_X");
      *x = event->value;
      break;
    case ABS_Y:
      strcpy(code, "ABS_Y");
      *y = event->value;
      break;
    default:
      strcpy(code, "ABS_OTHER");
      break;
    }
  } else if (event->type == EV_KEY) {
    switch (event->code) {
    case BTN_LEFT:
      strcpy(code, "BTN_LEFT");
      break;
    case BTN_RIGHT:
      strcpy(code, "BTN_RIGHT");
      break;
    case BTN_MIDDLE:
      strcpy(code, "BTN_MIDDLE");
      break;
    default:
      strcpy(code, "KEY_OTHER");
      break;
    }
  } else {
    strcpy(code, "OTHER");
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage: %s <event-file-path>\n", argv[0]);
    return 0;
  }

  printf("Device %s is begin intercepted...\n", argv[1]);

  /*Relative Postion*/
  int x = 0, y = 0;

  /*Use the struct like a buffer*/
  struct input_event event;
  int fd;
  char *event_name = argv[1];
  char inputpath[64];
  snprintf(inputpath, sizeof(inputpath), "/dev/input/%s", event_name);
  struct tm *info;
  char time_buffer[128];
  /*Open the data DEVICE_FILE*/

  fd = open(inputpath, O_RDONLY);
  if (fd < 0) {
    perror("Error to read a file inpunt");
    return 0;
  }
  while (1) {
    int bytes_read = read(fd, &event, sizeof(event));
    if (bytes_read < (ssize_t)sizeof(event))
      continue;

    printf("Event recived:\n");

    info = localtime(&event.time.tv_sec);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", info);

    // Print the time field
    printf("Time: %s.%06ld\n", time_buffer, event.time.tv_usec);
    char code[15];
    get_code(&event, code, &x, &y);
    printf("Code: %s\n", code);
    printf("Position x: %d, y: %d\n", x, y);
    printf("Value: %d\n", event.value);

    printf("-------------------------------------\n");
  }

  close(fd);
  return 1;
}

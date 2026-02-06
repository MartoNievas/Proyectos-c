#include <errno.h>
#include <fcntl.h>
#include <libnotify/notify.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

#define LOW_BATTERY_WARNING_THRESHOLD 20
#define TLP_ACTIVATION_THRESHOLD 80
#define CHECK_INTERVAL 5 // seconds between checks

#define F_LOW 1
#define F_CHG 2
#define F_FULL 4

volatile sig_atomic_t run = 1;

void stop_handler(int sig) {
  (void)sig;
  run = 0;
}

char *readfile(char *base, char *file) {
  char path[512];
  char *line = malloc(512);
  if (!line)
    return NULL;

  memset(line, 0, 512);
  snprintf(path, sizeof(path), "%s/%s", base, file);

  FILE *fp = fopen(path, "r");
  if (!fp) {
    free(line);
    return NULL;
  }

  if (!fgets(line, 511, fp)) {
    fclose(fp);
    free(line);
    return NULL;
  }

  fclose(fp);

  // Remove newline
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n')
    line[len - 1] = '\0';

  return line;
}

void activate_tlp() { system("sudo tlp start 2>/dev/null"); }

void deactivate_tlp() { system("sudo tlp stop 2>/dev/null"); }

void send_notification(const char *title, const char *body,
                       NotifyUrgency urgency) {
  NotifyNotification *notify = notify_notification_new(title, body, NULL);
  if (notify) {
    notify_notification_set_urgency(notify, urgency);
    notify_notification_show(notify, NULL);
    g_object_unref(notify);
  }
}

int main() {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = stop_handler;
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);

  int cap = 0;
  int flag = 0;
  int last_charging_state = -1; // -1: unknown, 0: discharging, 1: charging
  int tlp_active = 0;

  char file[256];
  snprintf(file, sizeof(file), "%s/.lowbattery_lock", getenv("HOME"));

  int pid_file = open(file, O_CREAT | O_RDWR, 0666);
  if (pid_file < 0) {
    perror("Error opening lock file");
    return 1;
  }

  if (flock(pid_file, LOCK_EX | LOCK_NB)) {
    if (errno == EWOULDBLOCK) {
      fprintf(stderr, "Another instance is already running\n");
      close(pid_file);
      return 1;
    }
  }

  char path[64] = {0};
  FILE *fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
  if (fp) {
    strcpy(path, "/sys/class/power_supply/BAT0");
    fclose(fp);
  } else {
    fp = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    if (fp) {
      strcpy(path, "/sys/class/power_supply/BAT1");
      fclose(fp);
    } else {
      fprintf(stderr, "Battery not found\n");
      close(pid_file);
      return 1;
    }
  }

  if (!notify_init("Battery Monitor")) {
    fprintf(stderr, "Error initializing libnotify\n");
    close(pid_file);
    return 1;
  }

  while (run) {
    char *cp = readfile(path, "capacity");
    if (!cp) {
      sleep(CHECK_INTERVAL);
      continue;
    }

    sscanf(cp, "%d", &cap);
    free(cp);

    cp = readfile(path, "status");
    if (!cp) {
      sleep(CHECK_INTERVAL);
      continue;
    }

    int is_charging = 0;

    if (!strncmp(cp, "Discharging", 11)) {
      is_charging = 0;

      // Charger disconnected notification
      if (last_charging_state == 1) {
        send_notification("Charger Disconnected", "Battery discharging",
                          NOTIFY_URGENCY_NORMAL);
      }

      // Low battery warning
      if (cap <= LOW_BATTERY_WARNING_THRESHOLD && !(flag & F_LOW)) {
        flag |= F_LOW;
        send_notification("Battery Low", "Connect charger",
                          NOTIFY_URGENCY_CRITICAL);
      }

      // Deactivate TLP if it was active
      if (tlp_active) {
        deactivate_tlp();
        tlp_active = 0;
      }

      flag &= ~F_CHG;
      flag &= ~F_FULL;

    } else if (!strncmp(cp, "Charging", 8) ||
               !strncmp(cp, "Not charging", 12)) {
      is_charging = 1;

      // Charger connected notification
      if (last_charging_state == 0 || last_charging_state == -1) {
        send_notification("Charger Connected", "Battery charging",
                          NOTIFY_URGENCY_NORMAL);
      }

      if (flag & F_LOW)
        flag &= ~F_LOW;

      if (cap < 100)
        flag &= ~F_FULL;

      flag |= F_CHG;

      // Activate TLP if battery is above 80%
      if (cap > TLP_ACTIVATION_THRESHOLD && !tlp_active) {
        activate_tlp();
        tlp_active = 1;
        send_notification("TLP Activated", "Battery protection enabled (>80%)",
                          NOTIFY_URGENCY_LOW);
      }

      // Deactivate TLP if battery drops below 80%
      if (cap <= TLP_ACTIVATION_THRESHOLD && tlp_active) {
        deactivate_tlp();
        tlp_active = 0;
      }

      // Battery full
      if (cap == 100 && !(flag & F_FULL)) {
        flag |= F_FULL;
        send_notification("Battery Full", "You can disconnect the charger",
                          NOTIFY_URGENCY_NORMAL);
      }
    }

    last_charging_state = is_charging;
    free(cp);

    sleep(CHECK_INTERVAL);
  }

  if (tlp_active) {
    deactivate_tlp();
  }

  notify_uninit();
  close(pid_file);
  unlink(file);

  return 0;
}

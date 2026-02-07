#include <errno.h>
#include <fcntl.h>
#include <libnotify/notify.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

/* Thresholds and Configuration */
#define LOW_BATTERY_WARNING_THRESHOLD 20
#define TLP_INFO_THRESHOLD 80
#define CHECK_INTERVAL 5

#define F_LOW 1
#define F_CHG 2
#define F_FULL 4

volatile sig_atomic_t run = 1;

/* Signal handler to stop the loop gracefully */
void stop_handler(int sig) {
  (void)sig;
  run = 0;
}

/* Reads a single line from a file in /sys/class/power_supply/ */
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

  /* Remove trailing newline */
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n')
    line[len - 1] = '\0';
  return line;
}

/* Wrapper for libnotify alerts */
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
  /* Setup signal handling */
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = stop_handler;
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);

  int cap = 0;
  int flag = 0;
  int last_charging_state = -1;
  int tlp_notified = 0;

  /* Lock file logic to prevent multiple instances */
  char file[256];
  snprintf(file, sizeof(file), "%s/.lowbattery_lock", getenv("HOME"));
  int pid_file = open(file, O_CREAT | O_RDWR, 0666);
  if (pid_file < 0)
    return 1;
  if (flock(pid_file, LOCK_EX | LOCK_NB)) {
    close(pid_file);
    return 1;
  }

  /* Detect battery path (BAT0 or BAT1) */
  char path[64] = {0};
  if (access("/sys/class/power_supply/BAT0/capacity", F_OK) == 0)
    strcpy(path, "/sys/class/power_supply/BAT0");
  else
    strcpy(path, "/sys/class/power_supply/BAT1");

  if (!notify_init("Battery Monitor"))
    return 1;

  while (run) {
    char *cp_cap = readfile(path, "capacity");
    char *cp_stat = readfile(path, "status");
    if (!cp_cap || !cp_stat) {
      sleep(CHECK_INTERVAL);
      continue;
    }

    sscanf(cp_cap, "%d", &cap);

    /* Check if power is connected (Charging, Full, or Not Charging due to TLP)
     */
    int plugged_in = (strncmp(cp_stat, "Charging", 8) == 0 ||
                      strncmp(cp_stat, "Not charging", 12) == 0 ||
                      strncmp(cp_stat, "Full", 4) == 0);

    if (strncmp(cp_stat, "Discharging", 11) == 0) {
      /* Case: Battery discharging */
      if (last_charging_state == 1)
        send_notification("Charger Disconnected", "Running on battery power",
                          NOTIFY_URGENCY_NORMAL);

      if (cap <= LOW_BATTERY_WARNING_THRESHOLD && !(flag & F_LOW)) {
        flag |= F_LOW;
        send_notification("Battery Low",
                          "Critical level, please connect charger",
                          NOTIFY_URGENCY_CRITICAL);
      }
      tlp_notified = 0;
      last_charging_state = 0;
      flag &= ~F_CHG;
    } else {
      /* Case: Power plugged in */
      if (last_charging_state == 0 || last_charging_state == -1)
        send_notification("Charger Connected", "External power source detected",
                          NOTIFY_URGENCY_NORMAL);

      if (flag & F_LOW)
        flag &= ~F_LOW;

      /* TLP Info: Notifies when > 80% and plugged in */
      if (cap > TLP_INFO_THRESHOLD && !tlp_notified) {
        send_notification("TLP Active",
                          "Battery > 80%. Charging managed by the system.",
                          NOTIFY_URGENCY_NORMAL);
        tlp_notified = 1;
      }

      if (cap <= TLP_INFO_THRESHOLD)
        tlp_notified = 0;

      last_charging_state = 1;
      flag |= F_CHG;
    }

    free(cp_cap);
    free(cp_stat);
    sleep(CHECK_INTERVAL);
  }

  /* Cleanup before exit */
  notify_uninit();
  close(pid_file);
  unlink(file);
  return 0;
}

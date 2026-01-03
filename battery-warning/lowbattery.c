#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <libnotify/notify.h>

#define LOW_BATTERY_WARNING_THRESHOLD 20

static char *readfile(const char *base, const char *file)
{
    char path[512];
    char *line = malloc(512);
    if (!line)
        return NULL;

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
    return line;
}

int main(void)
{
    int cap = 0;

    /* ------------------------------------------------ */
    /* Single instance lock                             */
    /* ------------------------------------------------ */
    char lockfile[512];
    snprintf(lockfile, sizeof(lockfile), "%s/.lowbattery_lock", getenv("HOME"));

    int pid_file = open(lockfile, O_CREAT | O_RDWR, 0666);
    if (pid_file < 0)
        return 1;

    if (flock(pid_file, LOCK_EX | LOCK_NB)) {
        if (errno == EWOULDBLOCK)
            return 1;
    }

    /* ------------------------------------------------ */
    /* Detect battery path                              */
    /* ------------------------------------------------ */
    char bat_path[64];

    if (access("/sys/class/power_supply/BAT0", F_OK) == 0)
        strcpy(bat_path, "/sys/class/power_supply/BAT0");
    else if (access("/sys/class/power_supply/BAT1", F_OK) == 0)
        strcpy(bat_path, "/sys/class/power_supply/BAT1");
    else
        return 1;

    /* ------------------------------------------------ */
    /* Init notifications                               */
    /* ------------------------------------------------ */
    if (!notify_init("lowbattery"))
        return 1;

    NotifyNotification *notif = NULL;
    char last_status[32] = "";
    int low_sent = 0;

    /* ------------------------------------------------ */
    /* Main loop                                        */
    /* ------------------------------------------------ */
    for (;;) {
        char *cp = readfile(bat_path, "capacity");
        if (!cp) { sleep(1); continue; }
        sscanf(cp, "%d", &cap);
        free(cp);

        cp = readfile(bat_path, "status");
        if (!cp) { sleep(1); continue; }

        /* 🔴 CRÍTICO: eliminar newline */
        cp[strcspn(cp, "\n")] = 0;

        notif = NULL;

        /* ---------------- Eventos de estado ---------------- */

        /* Charger disconnected */
        if (!strcmp(last_status, "Charging") &&
            !strcmp(cp, "Discharging")) {

            notif = notify_notification_new(
                "Charger disconnected",
                "Running on battery",
                NULL
            );

        /* Charger connected */
        } else if (strcmp(last_status, cp) != 0 &&
                   !strcmp(cp, "Charging")) {

            notif = notify_notification_new(
                "Battery Charging",
                "Charger connected",
                NULL
            );
            low_sent = 0;

        /* Battery full */
        } else if (strcmp(last_status, cp) != 0 &&
                   !strcmp(cp, "Full")) {

            notif = notify_notification_new(
                "Battery Full",
                "Disconnect charger",
                NULL
            );
        }

        /* ---------------- Low battery ---------------- */

        if (!strcmp(cp, "Discharging") &&
            cap <= LOW_BATTERY_WARNING_THRESHOLD &&
            !low_sent) {

            low_sent = 1;
            notif = notify_notification_new(
                "Battery Low",
                "Connect charger",
                NULL
            );
        }

        if (notif) { 
            notify_notification_show(notif, NULL);
            g_object_unref(notif);
        }

        strncpy(last_status, cp, sizeof(last_status));
        free(cp);
        sleep(1);
    }

    notify_uninit();
    close(pid_file);
    return 0;
}

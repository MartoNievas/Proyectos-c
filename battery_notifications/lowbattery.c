#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <libnotify/notify.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define LOW_BATTERY_WARNING_THRESHOLD 20
#define F_LOW 1
#define F_CHG 2
#define F_FULL 4

char* readfile(char* base, char* file)
{
    char path[512];
    char *line = malloc(512);
    if (!line) return NULL;

    memset(line, 0, 512);
    snprintf(path, sizeof(path), "%s/%s", base, file);

    FILE* fp = fopen(path, "r");
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

int main()
{
    int cap;
    int flag = 0;

    char file[256];
    strcpy(file, getenv("HOME"));
    strcat(file, "/.lowbattery_lock");

    int pid_file = open(file, O_CREAT | O_RDWR, 0666);
    if (pid_file < 0) return 1;

    if (flock(pid_file, LOCK_EX | LOCK_NB)) {
        if (errno == EWOULDBLOCK)
            return 1;
    }

    char path[64] = {0};
    FILE* fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (fp) {
        strcpy(path, "/sys/class/power_supply/BAT0");
        fclose(fp);
    } else {
        strcpy(path, "/sys/class/power_supply/BAT1");
    }

    if (!notify_init("Low battery notification"))
        return 1;

    for (;; sleep(1)) {

        char* cp = readfile(path, "capacity");
        if (!cp)
            continue;

        sscanf(cp, "%d", &cap);
        free(cp);

        cp = readfile(path, "status");
        if (!cp)
            continue;

        if (!strncmp(cp, "Discharging", 11)) {

            if (cap <= LOW_BATTERY_WARNING_THRESHOLD && !(flag & F_LOW)) {
                flag |= F_LOW;
                NotifyNotification* notify =
                    notify_notification_new("Battery Low", "Connect charger", NULL);
                notify_notification_set_urgency(notify, NOTIFY_URGENCY_CRITICAL);
                notify_notification_show(notify, NULL);
                g_object_unref(notify);
            }

            if (flag & F_CHG) {
                flag &= ~F_CHG;
                flag &= ~F_FULL;
                NotifyNotification* notify =
                    notify_notification_new("Discharging", "Charger disconnect", NULL);
                notify_notification_show(notify, NULL);
                g_object_unref(notify);
            }

        } else if (!strncmp(cp, "Charging", 8)) {
            if ((flag & F_LOW))
                flag &= ~F_LOW;

            if (cap < 100)
                flag &= ~F_FULL; 

            if (!(flag & F_CHG)) {
                flag |= F_CHG;
                flag &= ~F_FULL;
                NotifyNotification* notify =
                    notify_notification_new("Charging", "Charger connect", NULL);
                notify_notification_show(notify, NULL);
                g_object_unref(notify);
            }

            

            if (cap == 100 && (flag & F_CHG) && !(flag & F_FULL)) {
                flag |= F_FULL;
                NotifyNotification* notify =
                    notify_notification_new("Battery full", "Charger still connected", NULL);
                notify_notification_show(notify, NULL);
                g_object_unref(notify);
            }
        }

        free(cp);   
    }

    notify_uninit();
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <libnotify/notify.h>

#define LOW_BATTERY_WARNING_THRESHOLD 20
#define F_LOW 1
#define F_CHG 2
#define F_FULL 4
char* readfile(char* base, char* file)
{
    char path[512];
    char *line = (char*) malloc(512 * sizeof(char));
    memset(line, 0, 512 * sizeof(char));

    snprintf(path, 512, "%s/%s", base, file);
    FILE* fp = fopen(path, "r");

    if(fp == NULL) {
        free(line);
        return NULL;
    }
    if(fgets(line, 511, fp) == NULL) {
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
    strcat(strcpy(file, getenv("HOME")), "/.lowbattery_lock");
    int pid_file = open(file, O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno)				// another instance is already running
        {
            printf("Error: flock error\n");
            return 1;
        }
    }

    // Check for path where battery data is saved
    // Some laptops will have it set in BAT0 & some in BAT1
    char path[64] = { 0 };
    FILE* fp = fopen("/sys/class/power_supply/BAT0", "r");
    if (fp != NULL)
        strcat(path, "/sys/class/power_supply/BAT0");
    else
        strcat(path, "/sys/class/power_supply/BAT1");

    if (fp)
        fclose(fp);
    NotifyNotification* notification = NULL;
    if (!notify_init("Low battery notification")) {
      printf("Error: notify_init \n");
      return 0;
    } 
    for (;;sleep(1)) 
    {
        // Read current battery capacity
        char* cp = readfile(path, "capacity");
        if (!cp) continue;
        sscanf(cp, "%d", &cap);
        free(cp);
 
        // Read current battery status (Charing/Discharging)
        cp = readfile(path, "status");
        if (!cp) continue;
        //UnSet flag FULL
        
        if(!strncmp(cp, "Discharging", 11))
        {
            // If discharing and batery cap is less than threshold then send low battey notif
            if(cap <= LOW_BATTERY_WARNING_THRESHOLD && (flag & F_LOW) == 0)
            {
                // Set flag to one which means notification has been sent and won't bother you again
                flag |= F_LOW;
    
                notification = notify_notification_new("Battery Low", "Connect charger", NULL);
                notify_notification_set_urgency(notification, NOTIFY_URGENCY_CRITICAL);
 
            } else if (cap > LOW_BATTERY_WARNING_THRESHOLD && (flag & F_CHG)) {
                flag &= ~F_CHG; // set to flag not charge
                notification = notify_notification_new("Discharging", "Charger disconnect",NULL); 
            }    
        } 
        else if(!strncmp(cp, "Charging", 8))
        {
            if((flag & F_LOW))
            {
                flag &= ~F_LOW;
            }
            if (cap < 100 && (flag & F_FULL)) {
              flag &= ~F_FULL;
            }
            if ((flag & F_CHG) == 0) {
                flag |= F_CHG; //Set flag charge 
                notification = notify_notification_new("Charging", "Charger connect",NULL); 
            }
            if (cap == 100 && (flag & F_FULL) == 0) {
                flag |= F_FULL;
                notification = notify_notification_new("Battery full", "Disconnect charger",NULL);
            }

        } 
        if (notification)
        {   
            notify_notification_show(notification,NULL);
            g_object_unref(notification);
				    notification = NULL;
        }
        free(cp);
    }
    notify_uninit();


    return 0;
}

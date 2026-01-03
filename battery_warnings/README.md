## Prerequisite
* libnotify (make dependency)

## Steps to compile and use
* `sudo make clean install` or `sudo make clean install service` if you also want to install the service file
* `systemctl enable lowbattery.service`
* `systemctl start lowbattery.service`

## Prerquisitos
* libnotify (make dependency)

## Pasos para compilar y usar
* `sudo make clean install` o `sudo make clean install service` Solo si deseas instalar el archivo de service
* `systemctl enable lowbattery.service`
* `systemctl start lowbattery.service`

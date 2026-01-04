.PHONY: all clean install uninstall

PREFIX = /usr/local
SRC = lowbattery.c
OBJ = $(SRC:.c=.o)

all: lowbattery

.c.o:
	gcc -O2 `pkg-config --cflags libnotify` -c $<

lowbattery: lowbattery.o
	gcc `pkg-config --libs libnotify` lowbattery.o -o lowbattery

clean:
	rm -f lowbattery $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f lowbattery $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/lowbattery
service:
	cp -f lowbattery.service ~/.config/systemd/user/lowbattery.service
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/lowbattery
	rm -f ~/.config/systemd/user/lowbattery.service

enable: 
	systemctl --user daemon-reload
	systemctl --user enable lowbattery.service

start:
	systemctl --user restart lowbattery.service

disable:
	systemctl --user stop lowbattery.service
	systemctl --user disable lowbattery.service

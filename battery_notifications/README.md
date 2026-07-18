# Battery Notifications

A small daemon that watches battery state and sends desktop notifications through libnotify. Based on [vedpatil611/lowbattery](https://github.com/vedpatil611), adapted and patched for memory leaks and a cleaner systemd unit.

## What it does

Every 5 seconds, it reads `/sys/class/power_supply/BAT0/capacity` and `.../status` directly (falling back to `BAT1` if `BAT0` doesn't exist). No D-Bus or UPower calls; it just reads sysfs.

- Discharging and capacity drops to 20% or below: a critical "Battery Low" notification (fires once per discharge cycle).
- Switches from charging to discharging: "Charger Disconnected."
- Switches from discharging to charging (or starts up already plugged in): "Charger Connected."
- Charging and capacity is above 80%: a "TLP Active" notification, fired once until capacity drops back to 80% or below. This is informational only. The program doesn't call or manage TLP in any way; it's just a label on the notification.

A lock file at `~/.lowbattery_lock` (via `flock`) keeps a second instance from running.

## Requirements

- libnotify (development headers, for building)
- A running D-Bus session and notification daemon (e.g. Dunst) to actually display notifications

## Building and installing

```bash
sudo make clean install   # builds and copies the binary to /usr/local/bin
make service enable start # installs and starts the systemd user service
```

`sudo` is only needed for the `install` step, to write into `/usr/local/bin`; the daemon itself never runs anything as root.

Other targets: `make` (build only), `make uninstall`, `make disable` (stop and disable the service).

## systemd service

`lowbattery.service` is a systemd **user** unit tied to `graphical-session.target`. It sets `DISPLAY` and `DBUS_SESSION_BUS_ADDRESS` so libnotify can reach your session's notification daemon. If your user ID isn't 1000, edit the `DBUS_SESSION_BUS_ADDRESS` line in `lowbattery.service` before installing (check with `id -u`).

## License

MIT. See `LICENSE`. Copyright is shared between the original author (vedpatil611, 2021) and this fork's changes (2026).

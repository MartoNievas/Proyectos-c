# Mouse Tracker

Reads raw input events straight from the Linux kernel's evdev layer and prints them.

## What it does

Opens a device file under `/dev/input/` and reads `struct input_event` records in a loop, printing the timestamp, event code (`REL_X`, `REL_Y`, `BTN_LEFT`, etc.), a running x/y position clamped to 1920x1080, and the raw event value. It talks to the kernel input layer directly; no X11 or Wayland involved.

## Requirements

Read access to the device file. That usually means running as root, or adding your user to the `input` group and re-logging in.

To find your device name, check `/proc/bus/input/devices` or run `libinput list-devices`. The `event11`/`event12` numbers in the source comments are specific to the author's ThinkPad T480 touchpad and trackpoint; yours will likely differ.

## Building

```bash
make   # produces `mouse_tracker`
```

## Usage

```bash
./mouse_tracker event11
```

Pass just the device name, not the full path; the program prepends `/dev/input/` itself.

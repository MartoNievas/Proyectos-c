# own

A minimal terminal emulator built on GTK3 and VTE.

## What it does

Creates a GTK window with a single VTE terminal widget, spawning `$SHELL` (falling back to `/bin/bash`). Configuration lives in a header/source pair (`config/config.h`, `config/config.c`), a nod to the suckless philosophy of config-via-recompile, though the code itself isn't derived from st.

Keybindings (all with Ctrl held):

| Keys | Action |
|---|---|
| Ctrl+Shift+C | Copy |
| Ctrl+Shift+V | Paste |
| Ctrl+= (Ctrl++) | Zoom in |
| Ctrl+- | Zoom out |
| Ctrl+0 | Reset zoom |

Font is set to "JetBrainsMono Nerd Font" at size 20, scaled down to 0.75. There are no tabs; it's a single window with a single terminal. Background/foreground colors are defined in `config/config.c`, but the call that applies them (`terminal_set_colors`) is commented out in `src/main.c`, so they currently have no effect. The audible bell is disabled.

## Requirements

- GTK3 (`gtk+-3.0`)
- VTE (`vte-2.91`)

## Building

```bash
make   # produces build/own
```

## Running

```bash
./build/own
```

# Keyboard Input Visualizer

Draws a keyboard on screen and highlights each key as it's pressed, alongside a live log of recent input.

## What it does

Lays out 87 keys across 6 rows, modeled on the author's ThinkPad T480, and colors a key red for as long as `IsKeyDown` reports it held. A log line at the top of the window accumulates printable characters and special key names (Esc, F1-F12, arrows, etc.) as you type, capped at roughly 115 characters before it clears itself.

The Esc key doesn't close the window; `SetExitKey(0)` disables raylib's default exit-key behavior so you can see Esc highlighted like any other key. Close the window from your window manager instead.

Some key codes (the `<>` key, PrtSc, RePag/AvPag) use raw raylib key-code numbers rather than named constants, and the exact layout is specific to a T480; other keyboards may not map cleanly onto it.

## Building

```bash
gcc keyboard_input.c -o keyboard_input1 -lraylib -lm -lpthread -ldl -lrt -lX11
```

(or just `make`, which runs the same command)

## Running

```bash
./keyboard_input1
```

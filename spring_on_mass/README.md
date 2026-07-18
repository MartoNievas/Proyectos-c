# Spring on Mass

A horizontal spring-mass-damper simulation.

## What it does

`F = -kx - cv`, integrated each frame with semi-implicit Euler using raylib's `GetFrameTime()`. Draws a floor, a zig-zag line representing the spring, and a mass block sliding back and forth as it settles.

It's a fixed simulation with hardcoded constants (spring constant 40, mass 10, damping 1, initial velocity 200); there's no keyboard or mouse input, just watch it oscillate and settle.

## Building

```bash
make   # produces `spring`
```

Manual build:

```bash
gcc -o spring spring.c -Wall -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Running

```bash
./spring
```

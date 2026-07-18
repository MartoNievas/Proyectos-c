# Particle Collision

A small 2D physics toy: 100 particles falling under gravity in a raylib window, bouncing off the walls and colliding elastically with each other.

## What it does

Each particle has a random radius, position, and velocity. Every frame:

- Gravity pulls particles down; they bounce off the four walls with a damping factor applied on impact.
- Every pair of particles is checked for overlap (`O(n²)`, no spatial partitioning); overlapping pairs get resolved with a mass-weighted elastic impulse, plus a positional correction to push them back apart.

There's no user interaction; it's a self-running simulation. FPS is shown in the corner.

## Building

```bash
make   # produces `collision`
```

Manual build:

```bash
gcc -o collision collision.c -Wall -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Running

```bash
./collision
```

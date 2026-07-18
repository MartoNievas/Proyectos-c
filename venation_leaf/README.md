# Leaf Venation

Procedural leaf vein growth using the space colonization algorithm.

## What it does

Based on [Runions et al., "Modeling and Visualization of Leaf Venation Patterns"](https://algorithmicbotany.org/papers/venation.sig2005.pdf). A single vein starts near the bottom of the window. Each time you press Space:

1. Every attractor point ("auxin") pulls the nearest vein tip toward it.
2. Each vein tip grows a short step in the averaged direction of the auxins pulling on it.
3. Auxins that ended up close enough to a vein are removed.
4. A new batch of 20 random auxins is sprayed across the window.

Repeated presses branch and extend the vein network outward toward the remaining auxins, producing a leaf-like pattern.

## Requirements

Bundled: a prebuilt raylib 5.5 binary lives in `raylib-5.5_linux_amd64/`, so you don't need raylib installed separately.

## Building

```bash
cc -o nob nob.c && ./nob
```

This uses [nob.h](https://github.com/tsoding/nob.h), a single-header, self-rebuilding build tool: `nob.c` compiles itself, then compiles `main.c` against the vendored raylib.

## Running

```bash
./main
```

Press Space repeatedly to grow the leaf.

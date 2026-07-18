# Function Plotter

Plots a mathematical expression, given as a command-line argument, over a fixed domain.

## What it does

Renders a 900x900 window with a cartesian grid and the graph of the expression you pass in. Uses tinyexpr to parse and evaluate the expression, and SDL2 to draw the plot pixel by pixel by sampling `x` from -20 to 20 in small steps.

The domain is fixed at -20 to 20 on both axes; there's no way to change it from the command line. Axis number labels are drawn with SDL2_ttf using a hardcoded font path (`/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf`); if that font isn't installed, the labels are silently skipped but the grid lines and plot still render.

## Requirements

- SDL2
- SDL2_ttf
- tinyexpr (bundled in `lib/`)

On Arch Linux:

```bash
sudo pacman -Sy sdl2 sdl2_ttf
```

## Building

```bash
make main   # produces `programa`
```

## Usage

```bash
./programa "sin(x)"
```

Any expression tinyexpr can parse works, e.g. `"x^2 - 3*x + 1"` or `"cos(x) * x"`.

![Example: sin(x)](img/sin.png)

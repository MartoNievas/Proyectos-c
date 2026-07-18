# Paint

A small SDL2 drawing canvas.

## What it does

Opens a 900x600 window with a freehand circular brush. There's no save/load and no separate shape tools (line, rectangle); just the brush.

- Click and drag anywhere below the top bar to draw.
- The top-left bar holds 8 color swatches; click one to change the brush color.
- Ctrl+= / Ctrl+- resize the brush radius (up to 40px).
- Ctrl+Z undoes the last stroke, backed by a 100-entry surface stack.

## Building

```bash
make main   # produces `paint`
```

Requires SDL2 and SDL2_ttf via pkg-config (SDL2_ttf is linked but not actually used by the code).

## Running

```bash
./paint
```

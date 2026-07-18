# C Projects Collection

A collection of small C projects covering systems programming, graphics, simulations, networking, and a toy language interpreter. Most of these were written to learn or practice a specific API or algorithm, so scope and polish vary from project to project. Some are one-file experiments, others have their own README with build details.

## Contents

- [Cryptography](#cryptography)
- [Graphics](#graphics)
- [Hardware & Input Testing](#hardware--input-testing)
- [Simulations](#simulations)
- [System Utilities](#system-utilities)
- [Networking](#networking)
- [Language Implementation](#language-implementation)
- [Building](#building)
- [License](#license)

---

## Cryptography

### AES Encryption Tool (`aes/`)

A from-scratch implementation of AES (Rijndael), supporting AES-128, AES-192, and AES-256, with an interactive CLI for encrypting and decrypting messages. Implements SubBytes, ShiftRows, MixColumns, AddRoundKey (and their inverses), key schedule expansion, and PKCS#7 padding, following FIPS 197.

It has no cipher mode implementation, so encryption is effectively ECB, and there's no IV, MAC, or constant-time guarantees. It's an educational implementation, not something to use for real data. See `aes/README.md` for the full API reference, build instructions, and test vectors.

**Build**: `make` (targets: `all`, `clean`, `run`)

---

## Graphics

### Function Plotter (`function_plotter/`)

Renders the graph of a mathematical expression, passed as a command-line argument, over a given domain. Uses SDL2 for the window, tinyexpr to parse and evaluate the expression, and SDL2_ttf to draw axis labels.

**Build**: `make main`
**Usage**: `./programa "sin(x)"`

### Paint (`paint/`)

A small SDL2 drawing canvas (900x600). Freehand circular brush with adjustable radius (Ctrl+/Ctrl-, up to 40px), an 8-color palette, and undo via Ctrl+Z backed by a 100-entry surface stack. There's no save/load and no separate shape tools (line/rectangle), just the brush.

**Build**: `make main`

---

## Hardware & Input Testing

### Keyboard Input Visualizer (`keyboard_input/`)

Draws a ThinkPad T480 keyboard layout (87 keys across 6 rows) in a raylib window and highlights each key as it's pressed, alongside a scrolling log of recent keypresses.

**Build**: `gcc keyboard_input.c -o keyboard_input1 -lraylib -lm -lpthread -ldl -lrt -lX11`

### Mouse Tracker (`mouse_tracker/`)

Reads raw input events directly from a Linux evdev device file (e.g. `/dev/input/event11`), passed as a command-line argument, and prints each event (REL_X/REL_Y, buttons, etc.) along with a running, clamped x/y position. It talks to the kernel input layer directly, not X11 or Wayland, so it needs read access to the device file. The event numbers in the source comments are specific to the author's ThinkPad T480.

**Build**: `make`
**Usage**: `./mouse_tracker event11` (pass just the device name; the program prepends `/dev/input/` itself)

---

## Simulations

### Spring on Mass (`spring_on_mass/`)

A horizontal spring-mass-damper simulation in raylib: `F = -kx - cv`, integrated with semi-implicit Euler each frame. Draws the floor, a zig-zag spring, and the moving mass.

**Build**: `gcc -o spring spring.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11`

### Particle Collision (`collison/`)

100 particles falling under gravity inside a raylib window, bouncing off the walls with damping and resolving collisions between each other with mass-weighted elastic impulses. Collision checks are brute-force all-pairs (O(n²)); there's no spatial partitioning.

**Build**: `make`

### Leaf Venation (`venation_leaf/`)

Procedural leaf vein generation using the space colonization algorithm from [Runions et al., "Modeling and Visualization of Leaf Venation Patterns"](https://algorithmicbotany.org/papers/venation.sig2005.pdf). Each Space keypress sprays a batch of random attractor points, grows the veins toward their nearest attractors, and removes attractors once a vein gets close enough to them. Built with raylib and raymath, vendoring a prebuilt raylib 5.5 binary.

**Build**: `cc -o nob nob.c && ./nob` (uses [nob.h](https://github.com/tsoding/nob.h), a self-rebuilding single-header build tool)

---

## System Utilities

### File Splitter (`file_splitter/`)

Splits a file into N parts and rejoins them. Despite what its own README says, it's plain C (compiled with `gcc`, not a C++17 compiler) with no external dependencies. There's no checksum verification or progress indication, just chunked reads and writes through a fixed 1024-byte buffer.

**Build**: `make main`
**Usage**: `./file_splitter -s video.mp4 3` to split, `./file_splitter -j video.mp4.parte000 video.mp4.parte001 video.mp4.parte002 video.mp4` to rejoin

### Timer (`timer/`)

A single-shot countdown timer. Installs a `SIGALRM` handler with `sigaction()` and drives the countdown with `setitimer(ITIMER_REAL, ...)` at one-second intervals, blocking on `pause()` between ticks.

**Build**: `make`
**Usage**: `./timer <seconds>`

### Own Terminal (`own/`)

A minimal terminal emulator built on GTK3 and VTE (`vte_terminal_new`, spawning `$SHELL`). Configuration lives in a header file (`config/config.h`), a nod to suckless's philosophy, but the code itself isn't derived from st. Supports copy/paste (Ctrl+Shift+C/V) and zoom (Ctrl+/Ctrl-/Ctrl+0). No tabs; custom colors are defined in the config but the call that applies them is currently commented out in `main.c`.

**Build**: `make` (requires `gtk+-3.0` and `vte-2.91` via pkg-config)

### Battery Notifications (`battery_notifications/`)

A small daemon that polls `/sys/class/power_supply/BAT0` (or `BAT1`) every 5 seconds and sends desktop notifications via libnotify: a critical warning when discharging at or below 20%, and a status notification when charging above 80% (labeled "TLP Active," informational only; it doesn't call or manage TLP). Uses a lock file to prevent multiple instances and ships a systemd user service for autostart. `sudo` is only needed for `make install`, which copies the binary to `/usr/local/bin`; the daemon itself never runs commands as root. MIT licensed, based on [vedpatil611's lowbattery](https://github.com/vedpatil611).

**Build**: `sudo make clean install`, then `make service enable start`

### Disk Benchmark (`benchmark_disk/`)

Times sequential disk throughput by writing a 512MB file in 4MB blocks with `open()`/`write()`/`fsync()`, using `clock_gettime()` to measure elapsed time and report MB/s. `-w` times the write only; `-r` writes then times a sequential read. It's sequential-only, with a single fixed block size: no random I/O, IOPS, latency percentiles, or CSV/JSON export.

**Build**: `make`

---

## Networking

### UDP Message (`udp_message/`)

A UDP messaging tool with three modes: listen (`-l <port>`), send (`-d <ip> -p <port>`), and a bidirectional "complete" mode (`-d <ip> -l <port>`, multiplexed with `select()`). It's unicast only; broadcast, multicast, and raw sockets are listed as future work in the project's own README, not implemented. MIT licensed. See `udp_message/README.md` for usage examples.

**Build**: `gcc -o udp udp.c -Wall`

---

## Language Implementation

### Lambda Calculus Language (`lamb_calculus_languaje/`)

A REPL for the untyped lambda calculus, with a hand-written lexer and recursive-descent parser for `\x.body` and application syntax. Beta-reduces in normal order (leftmost-outermost, reducing under lambdas), printing each step up to a cap of 10. Bound variables are alpha-renamed to unique global IDs to avoid capture. This isn't true De Bruijn indexing, since the IDs don't encode binder depth or position.

The `nob.c`/`nob.h` files in this directory are a stray copy left over from `venation_leaf/` and don't build anything here (they reference a `main.c` and a raylib directory that don't exist in this project).

**Build**: `cc -o lamb lamb.c`

---

## Building

Each project builds independently with its own `makefile`/`Makefile`; there's no top-level build script. Most expose a `main` or `all` target (check the individual file; a couple use `make main` where others default to plain `make`). Two projects (`venation_leaf/`, and nominally `lamb_calculus_languaje/`, though its copy is broken) use tsoding's [nob.h](https://github.com/tsoding/nob.h) pattern instead: `cc -o nob nob.c && ./nob`.

Dependencies vary by project:

| Library | Used by |
|---|---|
| SDL2 / SDL2_ttf | `function_plotter`, `paint` |
| raylib | `collison`, `keyboard_input`, `spring_on_mass`, `venation_leaf` (vendored) |
| GTK3 + vte-2.91 | `own` |
| libnotify | `battery_notifications` |
| POSIX only | `aes`, `benchmark_disk`, `file_splitter`, `lamb_calculus_languaje`, `mouse_tracker`, `timer`, `udp_message` |

---

## License

There's no repository-wide license. `battery_notifications` and `udp_message` ship their own MIT `LICENSE` files (`aes/README.md` references one, but no `LICENSE` file is actually present in that directory); everything else currently has no license attached.

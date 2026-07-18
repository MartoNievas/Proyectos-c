# Timer

A single-shot countdown timer for the terminal.

## What it does

Installs a `SIGALRM` handler with `sigaction()` and drives a one-second tick with `setitimer(ITIMER_REAL, ...)`, blocking on `pause()` between ticks. Each second it reprints the remaining count in place; when it hits zero it prints "Timer elapsed!" and exits.

This is one countdown per run, not a multi-timer tool.

## Building

```bash
make   # produces `timer`
```

## Usage

```bash
./timer 30
```

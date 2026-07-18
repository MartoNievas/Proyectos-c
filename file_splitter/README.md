# File Splitter

A command-line tool for splitting a file into parts and rejoining them later.

## What it does

Split mode divides a file into N roughly equal parts, named `<basename>.parteNNN` (`.parte000`, `.parte001`, ...), streamed through a fixed 1024-byte buffer. Join mode concatenates a list of parts, in the order given on the command line, into a new output file.

It's plain C, built with `gcc`; no external dependencies. There's no checksum verification and no progress bar, just a print line per part processed.

## Building

```bash
make main   # produces `file_splitter`
```

## Usage

Split a file into 3 parts:

```bash
./file_splitter -s video.mp4 3
```

This creates `video.mp4.parte000`, `video.mp4.parte001`, `video.mp4.parte002`.

Join them back together:

```bash
./file_splitter -j video.mp4.parte000 video.mp4.parte001 video.mp4.parte002 video.mp4
```

Part order on the command line matters; the join happens in the order you list them.

## Notes

- Works on both text and binary files.
- Make sure you have read/write permissions for all files involved.

# Disk Benchmark

A small POSIX tool for timing sequential disk throughput.

## What it does

Writes a 512 MB file in 4 MB blocks to `<path>.benchdisk.dat`, using `open()`/`write()`/`fsync()`, and reports the result in MB/s using `clock_gettime(CLOCK_MONOTONIC, ...)`.

- `-w`: times the write only.
- `-r`: writes the file first (untimed setup), then times a sequential read of it.

The temporary file is deleted (`unlink`) after each run.

This is sequential-only, at a single fixed block size. There's no random I/O, no IOPS measurement, no latency percentiles, and no CSV or JSON export; it just prints one MB/s number.

## Building

```bash
make main   # produces `bench`
```

## Usage

```bash
./bench <path-prefix> -w   # time a write
./bench <path-prefix> -r   # time a write, then a read
```

`<path-prefix>` should point at the disk or mount point you want to test, e.g. `./bench /mnt/usbdrive/test -w` writes to `/mnt/usbdrive/test.benchdisk.dat`.

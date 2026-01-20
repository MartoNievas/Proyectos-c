# File Splitter

file_splitter is a command-line utility that allows you to split a file into multiple parts
and reconstruct the original file from those parts.

This tool is useful for handling large files or performing segmented file transfers.

---

## Usage

### Split a file into parts

Splits a file into N parts.

./file_splitter -s <file_path> <num_parts>

Example:
./file_splitter -s video.mp4 3

This will generate:
video.mp4.part0
video.mp4.part1
video.mp4.part2

---

### Join file parts

Joins multiple file parts into a single output file.

./file_splitter -j <part1> <part2> ... <output_name>

Example:
./file_splitter -j video.mp4.part0 video.mp4.part1 video.mp4.part2 video.mp4

---

## Build Instructions

This project uses a Makefile to compile the source code.

To build the executable, run:

make main

After a successful build, the following executable will be generated:

file_splitter

---

## Requirements

- A C++ compiler with C++17 support (e.g. g++)
- make

---

## Output

- Generated file parts follow this naming convention:
  <original_filename>.partN
- The reconstructed file will have the name specified by the user.

---

## Notes

- The program supports both binary and text files.
- When using the -j option, the order of the parts matters.
- Make sure you have read/write permissions for the involved files.



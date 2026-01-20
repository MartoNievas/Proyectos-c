#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024 // Buffer size in bytes

/*
 * Splits a file into 'num' parts using a fixed-size buffer.
 * The last part will contain any remaining bytes.
 * Parameters:
 *   - file: pointer to the file to split
 *   - num: number of parts to divide the file into
 *   - base_name: base name for the output files
 */
void split(FILE *file, int num, char *base_name) {
  if (num <= 0) {
    printf("Error: Number of parts must be greater than 0\n");
    return;
  }

  // Calculate file size
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  // Calculate how many bytes each partition should have
  long partition_size = size / num;
  long remaining = size % num;

  printf("File size: %ld bytes\n", size);
  printf("Partition size: %ld bytes\n", partition_size);
  printf("Extra bytes for last partition: %ld bytes\n", remaining);

  // Allocate fixed-size buffer for reading/writing
  unsigned char *buffer = malloc(BLOCK_SIZE);
  if (!buffer) {
    printf("Error: Cannot allocate memory\n");
    return;
  }

  // Process each partition
  for (int part = 0; part < num; part++) {
    char name[64];
    snprintf(name, sizeof(name), "%s.parte%03d", base_name, part);

    FILE *out = fopen(name, "wb");
    if (!out) {
      printf("Error: Cannot create output file %s\n", name);
      free(buffer);
      return;
    }

    // Calculate bytes to write for this partition
    // Last partition gets the remaining bytes too
    long bytes_to_write = partition_size;
    if (part == num - 1) {
      bytes_to_write += remaining;
    }

    long bytes_written = 0;

    // Read and write in BLOCK_SIZE chunks
    while (bytes_written < bytes_to_write) {
      // Determine how many bytes to read in this iteration
      long bytes_to_read = BLOCK_SIZE;
      if (bytes_to_write - bytes_written < BLOCK_SIZE) {
        bytes_to_read = bytes_to_write - bytes_written;
      }

      // Read from source file
      size_t bytes_read = fread(buffer, 1, bytes_to_read, file);
      if (bytes_read == 0) {
        break; // End of file or error
      }

      // Write to partition file
      fwrite(buffer, 1, bytes_read, out);
      bytes_written += bytes_read;
    }

    fclose(out);
    printf("Created: %s (%ld bytes)\n", name, bytes_written);
  }

  free(buffer);
}

/*
 * Joins multiple file parts into a single file.
 * Parameters:
 *   - argc: argument count
 *   - argv: argument vector
 *     Format: program part1 part2 ... partN output_file -j
 */
void join(int argc, char *argv[]) {
  // Allocate fixed-size buffer
  unsigned char *buffer = malloc(BLOCK_SIZE);
  if (!buffer) {
    printf("Error: Cannot allocate memory\n");
    return;
  }

  // Output file is second to last argument
  char *filejoin_path = argv[argc - 2];
  FILE *filejoin = fopen(filejoin_path, "wb");
  if (!filejoin) {
    printf("Error: Cannot create output file %s\n", filejoin_path);
    free(buffer);
    return;
  }

  // Process each input file part
  for (int i = 1; i < argc - 2; i++) {
    char *filepart_path = argv[i];

    FILE *filepart = fopen(filepart_path, "rb");
    if (!filepart) {
      printf("Error: Cannot open input file %s\n", filepart_path);
      continue;
    }

    // Read and write in BLOCK_SIZE chunks
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BLOCK_SIZE, filepart)) > 0) {
      fwrite(buffer, 1, bytes_read, filejoin);
    }

    fclose(filepart);
    printf("Joined: %s\n", filepart_path);
  }

  fclose(filejoin);
  free(buffer);
  printf("Output file created: %s\n", filejoin_path);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage:\n");
    printf("  Split: %s <file> <num_parts> -s\n", argv[0]);
    printf("  Join:  %s <part1> <part2> ... <output_file> -j\n", argv[0]);
    return -1;
  }

  char *flag = argv[argc - 1];

  // Split mode
  if (strcmp(flag, "-s") == 0) {
    if (argc < 4) {
      printf("Error: Split mode requires filename and number of parts\n");
      return -1;
    }

    char *fpath = argv[1];
    int num = atoi(argv[2]);

    FILE *file = fopen(fpath, "rb");
    if (!file) {
      printf("Error: Cannot open file %s\n", fpath);
      return -1;
    }

    // Make a copy of fpath for basename since it may modify the string
    char *fpath_copy = strdup(fpath);
    split(file, num, basename(fpath_copy));
    free(fpath_copy);
    fclose(file);

    return 0;
  }

  // Join mode
  if (strcmp(flag, "-j") == 0) {
    if (argc < 5) {
      printf(
          "Error: Join mode requires at least 2 parts and output filename\n");
      return -1;
    }

    join(argc, argv);
    return 0;
  }

  printf("Error: Unknown flag '%s'\n", flag);
  printf("Use -s for split or -j for join\n");
  return -1;
}

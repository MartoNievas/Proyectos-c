#include <stddef.h>
#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_SIZE_MB 512
#define BLOCK_SIZE (4 * 1024 * 1024)

int write_file(char *filepath, void *buffer, size_t blocks) {
  if (!filepath || !buffer || blocks == 0) {
    printf("Invalid argumentes");
    return 0;
  }

  int fd = open(filepath, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (fd < 0) {
    printf("Invalid %s path\n", filepath);
    return 0;
  }

  /*Write file*/
  for (size_t i = 0; i < blocks; i++) {
    if (write(fd, buffer, BLOCK_SIZE) < 0) {
      perror("Error: Dont Write the file");
      return 0;
    }
  }
  if (fsync(fd) < 0) {
    printf("Error: Failure to call fsync");
    close(fd);
    return 0;
  }
  close(fd);
  return 1;
}

double time_elepsed(struct timespec start, struct timespec end) {
  return (double)(end.tv_sec - start.tv_sec) +
         ((double)(end.tv_nsec - start.tv_nsec) / 1e9);
}

int main(int argc, char *argv[]) {
  /*Arguements */
  if (argc != 3) {

    printf("Error: Arguments are missing");
    return 0;
  }

  char *disk = argv[1];

  char filepath[512];
  snprintf(filepath, sizeof(filepath), "%s.benchdisk.dat", disk);

  size_t total_bytes = (size_t)(FILE_SIZE_MB) * 1024 * 1024;
  size_t blocks = total_bytes / (size_t)BLOCK_SIZE;

  /*Buffer of write and read benchmark*/
  char *buffer = aligned_alloc(4096, BLOCK_SIZE);
  if (!buffer) {
    printf("Failure to allocate memory");
    return 0;
  }
  memset(buffer, 'A', BLOCK_SIZE);

  printf("Path: %s\n", argv[1]);
  printf("File: %s\n", filepath);
  printf("Size: %d MB\n", FILE_SIZE_MB);
  printf("Block size: %d bytes\n\n", BLOCK_SIZE);

  struct timespec start, end;
  printf("This is a benchmark\n");

  /*Body of benchmark*/
  if (strcmp("-r", argv[2]) == 0) {
    /*Read benchmark*/

    /*The first step is write the file to read*/
    if (!write_file(filepath, buffer, blocks))
      return 0;

    /*Next step is read the file*/
    int fd = open(filepath, O_RDONLY);
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < blocks; i++) {
      if (read(fd, buffer, BLOCK_SIZE) < 0) {
        perror("Error: Failure to read a file");
        free(buffer);
        close(fd);
        return 0;
      }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double read_time = time_elepsed(start, end);
    double read_mbps =
        ((double)(total_bytes) / (double)(1024 * 1024)) / read_time;
    printf("Read:  %.2f MB/s\n", read_mbps);

  } else if (strcmp("-w", argv[2]) == 0) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (!write_file(filepath, buffer, blocks))
      return 0;

    clock_gettime(CLOCK_MONOTONIC, &end);
    double write_time = time_elepsed(start, end);
    double write_mbps =
        ((double)(total_bytes) / (double)(1024 * 1024)) / write_time;
    printf("Write: %.2f MB/s\n", write_mbps);

  } else {

    printf("Argument %s is invalid", argv[2]);
    free(buffer);
    return 0;
  }

  unlink(filepath);
  free(buffer);
  return 1;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char* argv[]) 
{

  if (argc < 2)
  {
    printf("Error: Missing any arguments\n");
    exit(-1);;
  }

  int seconds = atoi(argv[1]);
  printf("Starting timer of %d seconds...\n",seconds);
  return 0;
} 

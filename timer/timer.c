#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char* argv[]) 
{

  if (argc != 2)
  {
    printf("Error: Missing any arguments\n");
    exit(-1);;
  }

  int seconds = atoi(argv[1]);
  printf("Starting timer of %d seconds...\n",seconds);
  
  struct timeval interval = {1,0};
  struct timeval remeaning = {1,0};
  struct itimerval itimer = {interval,remeaning};
  int code = getitimer(ITIMER_REAL,&itimer);
  printf("this is a code: %d\n",code);
  return 0;
} 

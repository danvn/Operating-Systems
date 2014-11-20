#include "benchmark.h"

/* Local Includes */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


/* Local Defines */

#define RADIUS (RAND_MAX / 2)

char resultStr[500];

/* Local Functions */
inline double dist(double x0, double y0, double x1, double y1){
    return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
}

inline double zeroDist(double x, double y){
    return dist(0, 0, x, y);
}

int pi(long iterations, int fd){
  long i;
  double x, y;
  double inCircle = 0.0;
  double inSquare = 0.0;
  double pCircle = 0.0;
  double result = 0.0;

  
  /* Calculate pi using statistical methode across all iterations*/
  for(i=0; i<iterations; i++){
    x = (random() % (RADIUS * 2)) - RADIUS;
    y = (random() % (RADIUS * 2)) - RADIUS;
    if(zeroDist(x,y) < RADIUS){
      inCircle++;
    }
    inSquare++;
  }
  
  /* Finish calculation */
  pCircle = inCircle/inSquare;
  result = pCircle * 4.0;

  //sprintf(resultStr, "%d\n", result); 
  write(fd, resultStr, strlen(resultStr)*sizeof(char));
  return result;
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    perror("Not enough Argument"); 
    exit(1); 
  }
  int processNum = atoi(argv[1]); 
  int fd;
  int policy;
  struct sched_param param;
  if(argc > 2){
    if(!strcmp(argv[2], "SCHED_OTHER")){
      policy = SCHED_OTHER;
    }
    else if(!strcmp(argv[2], "SCHED_FIFO")){
      policy = SCHED_FIFO;
    }
    else if(!strcmp(argv[2], "SCHED_RR")){
      policy = SCHED_RR;
    }
    else{
      fprintf(stderr, "Unhandeled scheduling policy\n");
      exit(EXIT_FAILURE);
    }
  }
  /* Set process to max prioty for given scheduler */
  param.sched_priority = sched_get_priority_max(policy);
  
  /* Set new scheduler policy */
  fprintf(stdout, "Current Scheduling Policy: %d\n", sched_getscheduler(0));
  fprintf(stdout, "Setting Scheduling Policy to: %d\n", policy);
  if(sched_setscheduler(0, policy, &param)){
    perror("Error setting scheduler policy");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "New Scheduling Policy: %d\n", sched_getscheduler(0));
  //setScheduler(argv[2]); 
  int pid = forkMe(processNum);
  
  if(pid == 0)
    {
      return 0; 
  }
  
  char filename[20];
  sprintf(filename, "test%d.txt", pid);
  fd = open(filename,O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  
  pi(1000000, fd); 
  
  close(fd);
  remove( filename );
  return 0; 
}

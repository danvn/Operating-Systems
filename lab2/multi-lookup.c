/*
  File: multi-lookup.c
  Author: Luke Woodruff
  Date: 10-12-2014
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>	//for usleep

#include "util.h"
#include "queue.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define RESOLVER_THREAD_COUNT 8

// Global variables
queue hostnameQueue;
pthread_mutex_t queueLock;
pthread_mutex_t fileLock;
FILE* outputfp = NULL;		//Holds the output file
int requestThreadsFinished = 0;

void* Requester(void* inputFileName)
{
  char hostname[SBUFSIZE];	//Holds the individual hostname
  char* payload;
  FILE* inputFile = fopen(inputFileName, "r");
  
  /* Read File and Process*/
  while(fscanf(inputFile, INPUTFS, hostname) > 0)
    {
      int queueSuccess = 0;
      while(queueSuccess == 0) 
	{
	  pthread_mutex_lock(&queueLock);
	  if(queue_is_full(&hostnameQueue))
	    {
	      pthread_mutex_unlock(&queueLock);
	      usleep( rand() % 100 ); //random sleep between 0 and 100 ms
	    }
	  else
	    {
	      payload = malloc(SBUFSIZE);
	      strncpy(payload, hostname, SBUFSIZE); 
	      queue_push(&hostnameQueue, payload);
	      pthread_mutex_unlock(&queueLock);
	      queueSuccess = 1;
	    }
	}
    }
  /* Close Input File */
  fclose(inputFile);
  return NULL;
}

void* Resolver()
{
  char* hostname;	//Should contain the hostname
  char firstipstr[INET6_ADDRSTRLEN]; //Holds the resolved IP address
  
  while(!queue_is_empty(&hostnameQueue) || !requestThreadsFinished)
    {
      pthread_mutex_lock(&queueLock);
      if(!queue_is_empty(&hostnameQueue))
	{
	  hostname = queue_pop(&hostnameQueue);
	  
	  if(hostname != NULL)
	    {
	      pthread_mutex_unlock(&queueLock);
	      
	      if(dnslookup(hostname, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE)
			    {
			      fprintf(stderr, "dnslookup error: %s\n", hostname);
			      strncpy(firstipstr, "", sizeof(firstipstr));
			    }
	      pthread_mutex_lock(&fileLock);
	      fprintf(outputfp, "%s,%s\n", hostname, firstipstr);
	      pthread_mutex_unlock(&fileLock);
	    }
	  free(hostname);
	}
      else	//Queue is empty
	{
	  pthread_mutex_unlock(&queueLock);
	}
    }
  return NULL;
}

int main(int argc, char* argv[])
{
  int numFiles = argc - 2;
  /* Local Vars */
  
  pthread_mutex_init(&queueLock, NULL);
  pthread_mutex_init(&fileLock, NULL);
  
  queue_init(&hostnameQueue, 50);
  
  pthread_t requestThreads[argc-1];
  pthread_t resolverThreads[RESOLVER_THREAD_COUNT];
  
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  
  /* Check Arguments */
  if(argc < MINARGS)
    {
      fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
      fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
      return EXIT_FAILURE;
    }
    
  /* Open Output File */
  outputfp = fopen(argv[(argc-1)], "w");
  if(!outputfp)
    {
      perror("Error Opening Output File");
      return EXIT_FAILURE;
    }
  
  /* Loop Through Input Files */
  for(int i=1; i<(argc-1); i++)	//Allocate 1 thread for every iteration here
    {
      if(pthread_create(&requestThreads[i-1], &attr, Requester, argv[i]))
	{
	  printf("Request thread broke\n");
	}
    }
  
  /* Create resolver threads */
  for(int i = 0; i < RESOLVER_THREAD_COUNT; ++i)
    {
      if(pthread_create(&resolverThreads[i], &attr, Resolver, NULL))
    	{
	  printf("Resolver thread broke\n");
    	}
    }
  
  /* Join on the request threads */
  for(int i = 0; i < numFiles; ++i)
    {
      
      if(pthread_join( requestThreads[i],  NULL))
    	{
	  printf("Request thread broke");
    	}
    }
  requestThreadsFinished = 1;
  
  /* Join on the resolver threads */
  for(int i = 0; i < RESOLVER_THREAD_COUNT; ++i)
    {
      if(pthread_join( resolverThreads[i], NULL))
    	{
	  printf("Resolver thread broke");
    	}    
    }
  
  /* Close Output File */
  fclose(outputfp);
  queue_cleanup(&hostnameQueue);
  /* Destroy mutexes */
  pthread_mutex_destroy(&queueLock);
  pthread_mutex_destroy(&fileLock);
  
  return EXIT_SUCCESS;
}

#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>
#define SIZE 8

int opt, threadsprod, threadsconso;


int main(int argc, char ** argv) {

  while ((opt = getopt(argc, argv, "p:c:")) != -1) {
    switch (opt) {
    case 'j':
      threadsprod = atoi(optarg);
      break;
    case 'n':
      threadsconso = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s [-p threadsProd] [-c threadsConso]\n", argv[0]);
    }
  }
  
 
  
  return 0;
}
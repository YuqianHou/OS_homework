#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "mysem.h"

static int sem_id;

int main(int argc, char *argv[])
{
    int i;
    int pause_time;
	 char op_char = 'O';
    srand((unsigned int)getpid());
	 sem_id = sem_create(1234);
	 if (argc > 1) {
	    if (!sem_set(sem_id,1)) {
	        fprintf(stderr, "Failed to initialize semaphore\n");
	        exit(EXIT_FAILURE);
		    }
	    op_char = 'X';
	    sleep(2);
	 }
	for(i = 0; i < 10; i++) {
   	if (!sem_p(sem_id)) exit(EXIT_FAILURE);
    	printf("%c", op_char);fflush(stdout);
    	pause_time = rand() % 3;
    	sleep(pause_time);
    	printf("%c", op_char);fflush(stdout);
      if (!sem_v(sem_id)) exit(EXIT_FAILURE);
      pause_time = rand() % 2;
      sleep(pause_time);
  	}
  	printf("\n%d - finished\n", getpid());
  	if (argc > 1) {
      sleep(10);
      sem_del(sem_id);
  	}
  	exit(EXIT_SUCCESS);
}

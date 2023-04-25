#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define DATA 0
#define RESULT 1
#define FINISH 2

int generate(int x, int m){
	int a = 106;
  	int c = 1283;
	return (a*x+c)%m;
}

int main(int argc,char **argv) {

  Args ins__args;
  parseArgs(&ins__args, &argc, argv);

  int max_trials = ins__args.start; 
  int max_num = ins__args.stop;
  
  MPI_Status status;
  
  int result = 0, resulttemp = 0, trials = 0;
  int last[] = {0};
  int *hist;
  int seed[] = {0};
  int i;
  int sum_all = 0;
  
  srand(time(NULL));
  
  hist = (int*)calloc(max_num, sizeof(int));

  struct timeval ins__tstart, ins__tstop;

  int myrank,nproc;
  
  MPI_Init(&argc,&argv);

  // obtain my rank
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  // and the number of processes
  MPI_Comm_size(MPI_COMM_WORLD,&nproc);

  if(!myrank) 
      gettimeofday(&ins__tstart, NULL);

  // run your computations here (including MPI communication)
  
  if(myrank == 0){
  	for (i = 1; i < nproc; i++)
    {
      seed[0] = rand()%6075;
      MPI_Send (seed, 1, MPI_INTEGER, i, DATA, MPI_COMM_WORLD);
      trials++;
    }
    do
    {
        // distribute remaining subranges to the processes which have completed their parts
        MPI_Recv (&resulttemp, 1, MPI_INTEGER, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
        hist[resulttemp]++;
        MPI_Send (&resulttemp, 1, MPI_INTEGER, status.MPI_SOURCE, DATA, MPI_COMM_WORLD);
        trials++;
    }

    while (trials < max_trials);
    // now receive results from the processes
    for (i = 0; i < (nproc - 1); i++)
    {
      MPI_Recv (&resulttemp, 1, MPI_INTEGER, MPI_ANY_SOURCE, RESULT, MPI_COMM_WORLD, &status);
      hist[resulttemp]++;
    }
    // shut down the slaves
    for (i = 1; i < nproc; i++)
    {
      MPI_Send (NULL, 0, MPI_INTEGER, i, FINISH, MPI_COMM_WORLD);
    }
    // now display the result
    printf ("\nHi, I am process 0, the result is %d\n", result);
    sum_all = 0;
    for(i=0; i<max_num; i++){
      printf("num: %d -> %d\n", i, hist[i]);
      sum_all += hist[i];
    }
    printf("All numbers: %d\n", sum_all);
  }
  else	// slave
  {				
    do
    {
      MPI_Probe (0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == DATA)
      {
        MPI_Recv (seed, 1, MPI_INTEGER, 0, DATA, MPI_COMM_WORLD, &status);
        resulttemp = generate(seed[0], max_num);
        MPI_Send(&resulttemp, 1, MPI_INTEGER, 0, RESULT, MPI_COMM_WORLD);
      }
    }
    while (status.MPI_TAG != FINISH);
  }

  // synchronize/finalize your computations

  if (!myrank) {
    gettimeofday(&ins__tstop, NULL);
    ins__printtime(&ins__tstart, &ins__tstop, ins__args.marker);
  }
  free(hist);
  MPI_Finalize();
}

#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <omp.h>

#define RESULT 1

void calculate(int rank, int step){
  float x, y;
  int start = rank * step;
  int end = start + step;
  long total_in = 0;
  #pragma omp parallel for private(x, y) reduction(+:total_in)
    for(int i = start; i < end; i++){
      x = (float)rand()/RAND_MAX;
      y = (float)rand()/RAND_MAX;
      if((x*x+y*y) <= 1.0){
        total_in += 1;
      }
    }

  MPI_Send (&total_in, 1, MPI_LONG, 0, RESULT, MPI_COMM_WORLD);
}

int main(int argc,char **argv) {

  Args ins__args;
  parseArgs(&ins__args, &argc, argv);

  //set number of threads
  omp_set_num_threads(ins__args.n_thr);
  
  //program input argument
  long n = ins__args.arg; 

  struct timeval ins__tstart, ins__tstop;

  long total_in = 0;
  int threadsupport;
  int myrank,nproc;
  MPI_Status status;
  long res;

  // Initialize MPI with desired support for multithreading -- state your desired support level

  MPI_Init_thread(&argc, &argv,MPI_THREAD_FUNNELED,&threadsupport); 

  if (threadsupport<MPI_THREAD_FUNNELED) {
    printf("\nThe implementation does not support MPI_THREAD_FUNNELED, it supports level %d\n",threadsupport);
    MPI_Finalize();
    return -1;
  }
  
  // obtain my rank
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  // and the number of processes
  MPI_Comm_size(MPI_COMM_WORLD,&nproc);

  if (!myrank) 
    gettimeofday(&ins__tstart, NULL);

  int step = n/nproc;
  calculate(myrank, step);

  if(!myrank){
    for(int i=0; i < nproc; i++){
      MPI_Recv (&res, 1, MPI_LONG, i, RESULT, MPI_COMM_WORLD, &status);
      total_in += res;
    }
    printf("\nPI: %f\n", (double)total_in/n*4.0);
  }

  // synchronize/finalize your computations

  if (!myrank) {
    gettimeofday(&ins__tstop, NULL);
    ins__printtime(&ins__tstart, &ins__tstop, ins__args.marker);
  }
    
  MPI_Finalize();
  
}

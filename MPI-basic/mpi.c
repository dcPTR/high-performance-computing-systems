#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

int isPrime(long mine){
	for(int i=2; i<=sqrt(mine); i++){
		if(mine%i == 0){
			return 0; // has divisor
		}
	}
	return 1; // no divisors, it is Prime
}

int main(int argc,char **argv) {

  Args ins__args;
  parseArgs(&ins__args, &argc, argv);

  //program input argument
  long limit = ins__args.arg; 

  struct timeval ins__tstart, ins__tstop;

  int myrank, nproc;
  int primes = 0, primes_final = 0;
  int mine;
  
  MPI_Init(&argc,&argv);

  // obtain my rank
  MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
  // and the number of processes
  MPI_Comm_size(MPI_COMM_WORLD,&nproc);

  if(!myrank)
      gettimeofday(&ins__tstart, NULL);
 
  // run your computations here (including MPI communication)
	primes = 0;
	mine = myrank + 2;

	for (; mine <= limit;)
	{
	  primes += isPrime(mine);
	  mine += nproc;
	}

  MPI_Reduce (&primes, &primes_final, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

  // synchronize/finalize your computations

  if (!myrank) {
    printf ("\nNumber of primes between 2 and %ld is %d\n", limit, primes_final);
    gettimeofday(&ins__tstop, NULL);
    ins__printtime(&ins__tstart, &ins__tstop, ins__args.marker);
  }
  
  MPI_Finalize();

}

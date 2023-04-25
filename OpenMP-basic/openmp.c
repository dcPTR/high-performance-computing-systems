#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <time.h>

int main(int argc,char **argv) {
  Args ins__args;
  parseArgs(&ins__args, &argc, argv);

  omp_set_num_threads(ins__args.n_thr);
  
  long n = ins__args.arg; 
  long total_in = 0;
  srand(time(NULL));

  struct timeval ins__tstart, ins__tstop;
  gettimeofday(&ins__tstart, NULL);
  
  float x, y;
  #pragma omp parallel for private(x, y) reduction(+:total_in)
    for(int i = 0; i<n; i++){
      x = (float)rand()/RAND_MAX;
      y = (float)rand()/RAND_MAX;
      if((x*x+y*y) < 1.0){
        total_in += 1;
      }
    }
  
  printf("PI: %f\n", (double)total_in/n*4.0);
  
  // synchronize/finalize your computations
  gettimeofday(&ins__tstop, NULL);
  ins__printtime(&ins__tstart, &ins__tstop, ins__args.marker);
}

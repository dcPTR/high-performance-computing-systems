## OpenMP and MPI Monte Carlo PI Estimation

This program estimates the value of PI using a Monte Carlo simulation with OpenMP and MPI for parallel computing.

The program generates `n` random points within a square with side length 1.0, and calculates the number of generated points that fall within the quarter of the unit circle centered at `(0, 0)`. It then estimates the value of PI as four times the ratio of points within the quarter circle to the total number of generated points.

### Dependencies

* OpenMP and MPI libraries
* `utility.h` header file

### Usage

Compile the program with the OpenMP and MPI libraries using `make`. Then, run the executable with the number of points to generate, the marker, and the number of threads as command-line arguments using `make run`. For example:

```bash
make
make run 1000000 1 8
```

Where 1000000 is the number of points to generate, 1 is a marker, and 8 is the number of threads to use for parallelization. In this case, `mpirun` is used to launch the executable with 4 MPI processes. Note that the total number of threads used will be `nproc * n_thr`, where `nproc` is the number of MPI processes and `n_thr` is the number of OpenMP threads specified in the command-line arguments.

### Explanation

The `mpi+openmp.c` file contains the main program, which uses both MPI and OpenMP to parallelize the Monte Carlo simulation. The program first initializes MPI with `MPI_Init_thread` and obtains the rank of the current process and the total number of processes with `MPI_Comm_rank` and `MPI_Comm_size`, respectively.

The program then sets the number of threads to use with `omp_set_num_threads` and calculates the number of points to generate per process. Each process then calls the `calculate` function to perform the Monte Carlo simulation with OpenMP parallelization, and sends the result to the root process with `MPI_Send`.

The root process receives the results from each process with `MPI_Recv` and calculates the final estimate of PI. The program then prints the result to the console.

Finally, the program synchronizes the MPI processes with `MPI_Finalize`.
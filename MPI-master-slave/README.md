
## MPI Random Number Generator

This program generates a large number of random numbers using simple generator with the Message Passing Interface (MPI) library for parallel computing.

The program distributes the computation among the available MPI processes, and each process generates a sequence of random numbers using a seed obtained from the master process. The master process collects the generated numbers and stores them in a histogram.

### Dependencies

* MPI library
* `utility.h` header file

### Usage

Compile the program with the MPI library using `make`. Then, run the executable with the number of random numbers to generate, the maximum number which can be generated and a marker as command-line arguments using `make run`. For example:

```bash
make
make run 100000 6075 1
```

Where `100000` is the maximum number of random numbers to generate, `6075` is the maximum number which can be generated and `1` is a marker.
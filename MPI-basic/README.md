## MPI Prime Number Counter

This program counts the number of prime numbers between 2 and a given limit, using the MPI library for parallel computing.

The program receives a limit as input, and each process computes the number of primes in its range of numbers. The program then reduces the counts from all processes to obtain the total number of primes.

### Dependencies

* MPI library
* `utility.h` header file

### Usage

Compile the program with the MPI library using `make`. Then, run the executable with the limit and the marker as command-line arguments using `make run`. For example:

```bash
make
make run 100 1
```

Where `100` is the limit, and `1` is a marker.

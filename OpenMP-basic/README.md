
## OpenMP Monte Carlo PI Estimation

This program estimates the value of PI using a Monte Carlo simulation with OpenMP for parallel computing.

The program generates `n` random points within a square with side length 1.0, and calculates the number of generated points that fall within the quarter of the unit circle centered at `(0, 0)`. It then estimates the value of PI as four times the ratio of points within the quarter circle to the total number of generated points.

### Dependencies

* OpenMP library
* `utility.h` header file

### Usage

Compile the program with the OpenMP library using `make`. Then, run the executable with the number of points to generate, the marker, and the number of threads as command-line arguments using `make run`. For example:

```bash
make
make run 1000000 1 8
```

Where 1000000 is the number of points to generate, 1 is a marker, and 8 is the number of threads to use for parallelization.
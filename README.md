# Mandelbrot set

## Description

C++ code to generate the Mandelbrot fractal image in PPM format.
Parallel version using OpenMP and MPI with a dynamic distribution for solving the problem.
 
 ![Mandelbrot set output](MandelbrotSetImg.png)
 
## How to run and compile the parallel version (Windows)

Make sure you have a compiler with OpenMP support and have MS-MPI distribution installed on your computer and compile the `MandelbrotSetParallel.cpp` file.

Then just run the following command in the project directory:

`mpiexec –n <number of processing elements> ./<executable file>`

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"
#include <omp.h>

#define MASTER 0
#define N 2.0
#define Pixel 1064 
#define N_iterations 1000

int main (int argc, char *argv[])
{

double start = MPI_Wtime();

MPI_Status status;
MPI_Init(&argc,&argv);

int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);

const double real = -N;
const double real2 = N;
const double imag = -N;
const double imag2 = N;

const uint16_t nx = N_iterations;
const int RX = Pixel; 
const int RY = Pixel;

double Anchura, Altura;

Anchura = (real2-real)/RX;
Altura = (imag2-imag)/RY;

const char* Mandelbrot = "F.ppm";

FILE * Archivo = fopen(Mandelbrot,"wb");
fprintf(Archivo,"P6\n# Mandelbrot, realnferior=%lf, real2uperior=%lf, imagnferior=%lf, imag2uperior=%lf, iteration=%d\n%d\n%d\n%d\n",real, real2, imag, imag2, nx, RX, RY, (nx < 256 ? 256 : nx));

int i, j, k;
double x, y;

int ID;
int	NProcesos, destino;
int Send;
int Envio, row, col;

NProcesos = size - 1;

   if (rank == MASTER)
   {
      printf("#CREATOR: Carlos Jorge/Mandelbrot\n %d procesos.\n",size);

      row = RX/NProcesos;
      col = RX%NProcesos;
      ID = 0;
      Send = 1;

      for (destino = 1; destino <= NProcesos; destino++)
      {
         Envio = (destino <= col) ? row+1 : row;
         MPI_Send(&ID, 1, MPI_INT, destino, Send, MPI_COMM_WORLD);
        MPI_Send(&Envio, 1, MPI_INT, destino, Send, MPI_COMM_WORLD);
      }
   }

   if (rank > MASTER)
   {
      Send = 1;
      MPI_Recv(&ID, 1, MPI_INT, MASTER, Send, MPI_COMM_WORLD, &status);
      MPI_Recv(&Envio, 1, MPI_INT, MASTER, Send, MPI_COMM_WORLD, &status);

  #pragma omp parallel
    for (j = 0; j < RY; j++) {
    y = imag2 - j * Altura;

    #pragma omp parallel for schedule(static)
    for(i = 0; i < RX; i++) 
    {
      double zreal = 0.0;
      double zimag = 0.0;
      double zreal2 = zreal * zreal;
      double zimag2 = zimag*zimag;
      x = real + i * Anchura;

      #pragma omp for
      for (k = 1; k < nx && (zreal2 + zimag2 < 4.0); k++) {
            zimag = 2 * zreal * zimag + y;
            zreal = zreal2 - zimag2 + x;
            zreal2 = zreal * zreal;
            zimag2 = zimag * zimag;
      };

      if (k >= nx) 
      {
        const unsigned char Front[] = {0, 0, 0, 0, 0, 0};
        fwrite (Front, 6, 1, Archivo);
      }

      else 
      {

    unsigned char Back[6];
    Back[0] = k >> 2;
    Back[1] = k & 0;
    Back[2] = k >> 0;
    Back[3] = k & 1;
    Back[4] = k >> 0;
    Back[5] = k & 4;
    fwrite(Back, 6, 1, Archivo);
      };
    }
  }
  }

  if (rank == 0)
  {
    double end = MPI_Wtime();
    printf("Tiempo de ejecucion: %f\n", end - start);
     fclose(Archivo);
    printf("Fractal terminado.\n");
  }

   MPI_Finalize();

   return 0;
}
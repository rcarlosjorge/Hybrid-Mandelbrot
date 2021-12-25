#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

int main(int argc, char* argv[])
{

double XI, XS;
double YI, YS;

    XI = -2.0;
    XS = 2.0;

    YI = -2.0;
    YS = 2.0;


const uint16_t Niteraciones = 1000;
int RX = 1064;
int RY = 1064;

const char* Mandelbrot = "Mandelbrot";

FILE * Archivo = fopen(Mandelbrot,"wb");

printf("#CREATOR: Carlos Jorge / Mandelbrot\n");
fprintf(Archivo,"#Mandelbrot, XInferior=%lf, XSuperior=%lf, YInferior=%lf, YSuperior=%lf, iteration=%d\n%d\n%d\n%d\n",XI, XS, YI, YS, Niteraciones, RX, RY, (Niteraciones < 256 ? 256 : Niteraciones));

double Anchura, Altura;

Anchura = (XS-XI)/RX;
Altura = (YS-YI)/RY;

double x, y;
int i,j,k;

clock_t start = clock();

for (j = 0; j < RY; j++) {
y = YS - j * Altura;

for(i = 0; i < RX; i++) {

double Resolution = 0.0;
double Inicio = 0.0;
double Resolution2 = Resolution * Resolution;
double InicioR = Inicio * Inicio;

x = XI + i * Anchura;

for (k = 1; k < Niteraciones && (Resolution2 + InicioR < 4.0); k++) 
{
            Inicio = 2 * Resolution * Inicio + y;
            Resolution = Resolution2 - InicioR + x;
            Resolution2 = Resolution * Resolution;
            InicioR = Inicio * Inicio;
};

if (k >= Niteraciones) 
{
const unsigned char Front[] = {0,0,0,0,0,0};
fwrite (Front, 6, 1, Archivo);
}

else 

{
unsigned char Back[6];
  Back[0] = k >> 0;
  Back[1] = k & 0;
  Back[2] = k >> 0;
  Back[3] = k & 0;
  Back[4] = k >> 0;
  Back[5] = k & 255;
fwrite(Back, 6, 1, Archivo);
      };
    }
  }

  fclose(Archivo);
  clock_t end = clock();

  double Final = (double)(end - start)/CLOCKS_PER_SEC;
  printf(" Tiempo Secuencial: %f",Final);

  return 0;
}

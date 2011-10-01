#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>

int main (int argc, char *argv[])
{
  int i,j,k;
  float *a,*b,*c;
  struct timeval timev1,timev2;
  float time_seconds;

  int N = atoi ( argv [ 1 ] );
  int P = atoi ( argv [ 2 ] );
  printf ( "N=%d   P=%d\n", N, P );

  a = (float*) malloc(N*N*sizeof(float));
  b = (float*) malloc(N*N*sizeof(float));
  c = (float*) malloc(N*N*sizeof(float));

  for(i=0;i<N*N;i++) {
   a[i]=0.1;
   b[i]=0.2;
   c[i]=0.0;
  }

  omp_set_num_threads(P);

  gettimeofday(&timev1,NULL);

 #pragma omp parallel for
  for (i=0; i<N; i++)
   for (k=0; k<N; k++)
    for (j=0; j<N; j++)
     c[i*N+j]+=a[i*N+k]*b[k*N+j];

  gettimeofday(&timev2,NULL);
  time_seconds=timev2.tv_sec-timev1.tv_sec+0.000001*(timev2.tv_usec-timev1.tv_usec);

  printf("Computation time = %.4f\n",time_seconds);

  printf ( "c[0,0]=%f\n", c[0] );

  free(a);
  free(b);
  free(c);
}

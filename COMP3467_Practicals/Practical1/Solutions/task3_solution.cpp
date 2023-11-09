// --- Approximating pi via numerical integration ---
// The below program approximates the value of pi by numerically integrating 4/(1+x*x) in the interval [0,1] over N subintervals.
// a) Use the OpenMP routine omp_get_wtime() to measure the runtime of the seq_approximate_pi(...) function.
// b) Study the body of the for loop. How is it qualitatively different from the for loop of the vector addition with respect to parallelization? 
// c) Write a function gpu_approximate_pi(...) that parallelizes the given source code with OpenMP on a GPU. 
//    Consider the OpenMP documentation to identify the clause that can be used to alleviate the issue that you have discovered in task b). 
// d) Use the OpenMP routine omp_get_wtime() to measure the runtime of the gpu_approximate_pi(...) function, and compare it to the runtime
//    of the sequential function.

#include <stdio.h>
#include <omp.h>

#define num_subintervals 1000000000

void seq_approximate_pi()
{
  double x, pi, sum = 0.0;
  double subinterval_size = 1.0/num_subintervals;

  for (int i=0 ; i < num_subintervals; i++)
  {
    x = (i-0.5) * subinterval_size;
    sum = sum + 4.0/(1.0+x*x);
  }

  pi = subinterval_size * sum;
  printf("Pi on CPU: %.15lf.\n", pi);	
}

void gpu_approximate_pi()
{
  double x, pi, sum = 0.0;
  double subinterval_size = 1.0/num_subintervals;

  #pragma omp target teams distribute parallel for reduction(+:sum)
  for (int i=0 ; i < num_subintervals; i++)
  {
    x = (i-0.5) * subinterval_size;
    sum = sum + 4.0/(1.0+x*x);
  }

  pi = subinterval_size * sum;
  printf("Pi on GPU: %.15lf\n", pi);	
}

int main ()
{
  double start, stop, runtime;
  start = omp_get_wtime();
  seq_approximate_pi(); 
  stop = omp_get_wtime();
  runtime = stop - start; 
  printf("CPU runtime: %.15lf s\n", runtime);	

  start = omp_get_wtime();
  gpu_approximate_pi(); 
  stop = omp_get_wtime();
  runtime = stop - start; 
  printf("GPU runtime: %.15lf s\n", runtime);
}	
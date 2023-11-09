// --- Vector addition: the "Hello World" of parallel programming ---
// a) Study the functions vector_add_cpu_parallel_for(...) and vector_add_cpu_parallel_for_combined(...)
//    which implement a parallel vector addition on the CPU with OpenMP. Note that they are semantically equivalent.
// b) Fill in the "TODO" gaps by implementing a parallel vector addition on the GPU with each function exploiting specific levels of parallelism:
//      - vector_add_gpu_parallel_for_combined(...) shall run a single threadblock with multiple threads
//      - vector_add_gpu_teams_distribute_combined(...) shall run multiple threadblocks with a single thread
//      - vector_add_gpu_teams_parallel_combined(...) shall run multiple threadblocks each with multiple threads
//    Remember, that the memory of CPU and GPU are physically separated and accordingly explicit memory transfers are required.

#include <stdio.h>

#define N 10

void vector_add_cpu_parallel_for(double* X, double* Y, double* Z)
{

	#pragma omp parallel
	{
		#pragma omp for
		for(int i = 0; i < N; i++)
		{
			Z[i] = X[i] + Y[i];
		}
	}

}

void vector_add_cpu_parallel_for_combined(double* X, double* Y, double* Z)
{
	#pragma omp parallel for
	for(int i = 0; i < N; i++)
	{
		Z[i] = X[i] + Y[i];
	}
}

void vector_add_gpu_parallel_for_combined(double* X, double* Y, double* Z)
{
	// TODO
}

void vector_add_gpu_teams_distribute_combined(double* X, double* Y, double* Z)
{
	// TODO
}

void vector_add_gpu_teams_parallel_combined(double* X, double* Y, double* Z)
{
	// TODO
}


void init(double* X, double* Y, double* Z)
{
	for(int i = 0; i < N; i++)
	{
		X[i] = 1;
		Y[i] = 1;
		Z[i] = 0;
	}
}

void print_result(double* Z, const char* string)
{
    printf("%s\n",string);
    for(int i = 0; i < N; i++) printf("%lf ", Z[i]);
	printf("\n");
}

int main(int argc, char* argv[])
{
	double X[N], Y[N], Z[N];

	// CPU data parallelism
	init(X,Y,Z);
	vector_add_cpu_parallel_for(X,Y,Z);
    print_result(Z,"vector_add_cpu_parallel_for:");

	init(X,Y,Z);
	vector_add_cpu_parallel_for_combined(X,Y,Z);
	print_result(Z,"vector_add_cpu_parallel_for_combined:");

	// GPU data parallelism
	init(X,Y,Z);
	vector_add_gpu_parallel_for_combined(X,Y,Z);
	print_result(Z,"vector_add_gpu_parallel_for_combined:");

	init(X,Y,Z);
	vector_add_gpu_teams_distribute_combined(X,Y,Z);
	print_result(Z,"vector_add_gpu_teams_distribute_combined:");

	init(X,Y,Z);
	vector_add_gpu_teams_parallel_combined(X,Y,Z);
	print_result(Z,"vector_add_gpu_teams_parallel_combined:");
}
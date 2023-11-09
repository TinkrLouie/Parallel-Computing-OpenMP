// --- Implementing a parallel reduction with MPI ---
//
// The below sequential code sums up all elements in an array.
//
// a) Adjust the run_mpi_program.sh sbatch script such that it compiles and runs the code of task2.c.
//
// b) Parallelize the code with MPI. Each rank should compute the sum of all elements in a subarray, and afterwards 
//    send it's local result to rank 0 via MPI_Reduce to build up the total sum. You may assume that the
//    number of elements is evenly divisible by the number of ranks.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {

    const int N=12;
    int array[N];
    for(int i=0; i<N; i++)
    {
        array[i] = 1;
    }

    int sum=0;
    for(int i=0; i<N; i++)
    {
        sum += array[i];
    }

    printf("Total sum = %d\n", sum);
}

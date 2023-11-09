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

    MPI_Init(NULL, NULL);

    const int N=12;
    int array[N];
    for(int i=0; i<N; i++)
    {
        array[i] = 1;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    
    // Sum the numbers locally
    int local_sum = 0;
    int elements_per_rank = N/num_ranks;
    int start_index = elements_per_rank * rank;
    for (int i = start_index; i < (start_index + elements_per_rank); i++) {
        local_sum += array[i];
    }

    printf("Local sum for process %d is %d.\n", rank, local_sum);

    // Reduce all of the local sums into the global sum
    // Syntax:
    // int MPI_Reduce(	
    //                const void *sendbuf, 
    //                void *recvbuf, 
    //                int count, 
    //                MPI_Datatype datatype,
    //           	  MPI_Op op,
    //                int root,
    //                MPI_Comm comm
    //               )

    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
                MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum = %d\n", global_sum);
    }

    MPI_Finalize();
}

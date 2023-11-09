// --- Implementing a parallel Matrix-Vector multiplication with MPI ---
//
//  The below code implements a sequential Matrix-Vector multiplication:  A[M][N] x b[M] = x[N]
//     
//  a) Adjust the run_mpi_program.sh sbatch script such that it compiles and runs the code of task3.c.
//
//  b) Parallelize the code with MPI by making use of collective operations. You may assume that the
//     number of elements is evenly divisible by the number of ranks. 
//
//     Hint:  
//     You might want to start by revising the algorithm for matrix-vector multiplication, get acquainted 
//     with the data layout and reason about the operations that could be done in parallel. Draft the data 
//     layout of a parallel matrix-vector multiplication with pen and paper. 
//
//  c) How could you parallelize the code such that it works for an arbitrary number of ranks? Consider the different flavours
//     of collective operations, e.g., MPI_Gather vs MPI_Gatherv.
//

#include <stdio.h>
#include <mpi.h>

int main() {

    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_ranks;
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    const int rows = 4;
    const int cols = 2;
    double A[rows][cols];
    double b[cols];
    double x[rows];
    double global_x[rows];

    int my_num_rows = rows/num_ranks;
    int my_first_row = my_num_rows*rank;
    int my_last_row = my_first_row+my_num_rows;
    int displacements[num_ranks];
    int recvcounts[num_ranks];

    if(rows%num_ranks != 0 && rank==(num_ranks-1))
    {
        my_last_row += rows%num_ranks;
        my_num_rows += rows%num_ranks;
    }

    if(rank==0)
    {
        displacements[0]=0;
        for (int i = 1; i < num_ranks; i++)
        {
            displacements[i]=my_num_rows+displacements[i-1];
        }

        for (int i = 0; i < num_ranks; i++)
        {
            recvcounts[i] = my_num_rows;
        }
        recvcounts[num_ranks-1] = my_num_rows + rows%num_ranks;
    }

    for (int i = my_first_row; i < my_last_row; i++) {
        for (int j = 0; j < cols; j++) {
            A[i][j] = i * cols + j;
        }
    }

    for (int i = 0; i < cols; i++) {
        b[i] = 1;
    }

    for (int i = 0; i < rows; i++) {
        x[i] = 0;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = my_first_row; i < my_last_row; i++) {
        for (int j = 0; j < cols; j++) {
            x[i] += A[i][j] * b[j];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gatherv(&(x[my_first_row]), my_num_rows, MPI_DOUBLE,
               global_x, recvcounts, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("Global result vector x:\n");
        for (int i = 0; i < rows; i++) {
            printf("%f\n", global_x[i]);
        }
    }

    MPI_Finalize();
}

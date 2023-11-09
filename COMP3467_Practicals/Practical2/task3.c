// --- Implementing a parallel Matrix-Vector multiplication with MPI ---
//
//  The below code implements a sequential Matrix-Vector multiplication:  A[M][N] x b[M] = x[N]
//     
//
//  a) Adjust the run_mpi_program.sh sbatch script such that it compiles and runs the code of task3.c.
//
//  b) Parallelize the code with MPI by making use of collective operations. You may assume that the
//     number of elements is evenly divisible by the number of ranks. 
//
//     Hint:  
//     You might want to start by revising the algorithm for matrix-vector multiplication, get acquainted 
//     with the data layout and reason about the operations that could be done in parallel. Draft the data 
//     layout of a parallel matrix-vector multiplication with pen and paper. Consider the different flavours
//     of collective operations, e.g., MPI_Gather vs MPI_Gatherv.
//
//  c) How could you parallelize the code such that it works for an arbitrary number of ranks? Consider the different flavours
//     of collective operations, e.g., MPI_Gather vs MPI_Gatherv.
//

#include <stdio.h>

int main() {

    const int rows = 3;
    const int cols = 2;
    int A[rows][cols];
    int b[cols];
    int x[rows];

    printf("Input matrix A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            A[i][j] = i * cols + j;
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    printf("Input vector b:\n");
    for (int i = 0; i < cols; i++) {
        b[i] = 1;
        printf("%d\n", b[i]);
    }

    for (int i = 0; i < rows; i++) {
        x[i] = 0;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            x[i] += A[i][j] * b[j];
        }
    }

    printf("Result vector x:\n");
    for (int i = 0; i < rows; i++) {
        printf("%d\n", x[i]);
    }
}

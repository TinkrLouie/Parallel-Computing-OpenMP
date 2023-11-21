#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <cstdlib>
#include <unordered_set>
#define CHUNK_SIZE 32
// The generateMagicSquare() function is supposed to generate a large matrix square from two smaller ones.
//
// Input: 
// 1. A square matrix "pattern" of size N*N
// 2. A square matrix "modifier" of size N*N
//
// Output:
// A square matrix "magicSquare" of size M = (N*N) * (N*N)
//
// Algorithm:
// 1. Multiply each element of the "modifier" with M.
// 2. Copy the "pattern" matrix into each N*N square tile of the "magicSquare" matrix
//    such that the resulting matrix consists of N*N "pattern" blocks.
// 3. For all pattern blocks:
//      Multiply all elements in the "pattern" block[i][j] with the element at modifier[i][j]. 
//

void generateMagicSquare(int** pattern, int** modifier, int** magicSquare, int N, int M)
{   
    //if (N > 50) {
        #pragma omp target map(tofrom:magicSquare[:M][:M], modifier[:N][:N], pattern[:N][:N])
        {   
            if(omp_is_initial_device())
            {
              printf("Running on CPU\n");    
            }
            else{
                int num_teams= omp_get_num_teams(); 
                int num_threads_per_team = omp_get_num_threads();
                printf("Running on GPU with %d teams and %d threads per team\n", 
                  num_teams, 
                  num_threads_per_team
                );
            }     
        

            #pragma omp parallel for collapse(2) schedule(static, CHUNK_SIZE)
            for (int i = 0; i < N; i++)
            {   
                int num_teams= omp_get_num_teams(); 
                int num_threads_per_team = omp_get_num_threads();
                printf("Running on GPU with %d teams and %d threads per team\n", 
                  num_teams, 
                  num_threads_per_team
                );
                for (int j = 0; j < N; j++)
                {
	        	    modifier[i][j] *= M;
	            }
            }

            #pragma omp barrier

            //ORIGINAL APPROACH
            //#pragma omp parallel for collapse(2)
            //for (int i = 0; i < M; i++)
            //{
            //    for (int j = 0; j < M; j++)
            //    {
            //        int patternRow = i % N;
            //        int patternCol = j % N;
            //        magicSquare[i][j] = pattern[patternRow][patternCol];
	        //        magicSquare[i][j] += modifier[i/N][j/N];
            //    }
            //}

            //MATRIX TILING
            #pragma omp parallel for collapse(2) shared(magicSquare, pattern, modifier) schedule(static, CHUNK_SIZE)
            for (int iOuter = 0; iOuter < M; iOuter += CHUNK_SIZE)
            {
                for (int jOuter = 0; jOuter < M; jOuter += CHUNK_SIZE)
                {   
                    for (int i = iOuter; i < iOuter + CHUNK_SIZE && i < M; i++)
                    {   
                        int patternRow = i % N;
                        int modifierRow = i / N;
                        int* patternRowPtr = pattern[patternRow];
                        int* modifierRowPtr = modifier[modifierRow];
                        for (int j = jOuter; j < jOuter + CHUNK_SIZE && j < M; j++)
                        {
                            int patternCol = j % N;
                            int modifierCol = j / N;
                            magicSquare[i][j] = patternRowPtr[patternCol] + modifierRowPtr[modifierCol];
                        }
                    }
                }
            }
        }
    //}
    //else {
    //    #pragma omp parallel for collapse(2) schedule(dynamic)
    //    for (int i = 0; i < N; i++)
    //    {
    //        for (int j = 0; j < N; j++)
    //        {
	//    	    modifier[i][j] *= M;
	//        }
    //    }
//
    //    #pragma omp parallel for collapse(2) shared(magicSquare, pattern, modifier) schedule(dynamic)
    //    for (int iOuter = 0; iOuter < M; iOuter += CHUNK_SIZE)
    //    {
    //        for (int jOuter = 0; jOuter < M; jOuter += CHUNK_SIZE)
    //        {   
    //            for (int i = iOuter; i < iOuter + CHUNK_SIZE && i < M; i++)
    //            {   
    //                int patternRow = i % N;
    //                int modifierRow = i / N;
    //                int* patternRowPtr = pattern[patternRow];
    //                int* modifierRowPtr = modifier[modifierRow];
    //                for (int j = jOuter; j < jOuter + CHUNK_SIZE && j < M; j++)
    //                {
    //                    int patternCol = j % N;
    //                    int modifierCol = j / N;
    //                    magicSquare[i][j] = patternRowPtr[patternCol] + modifierRowPtr[modifierCol];
    //                }
    //            }
    //        }
    //    }
    //}
}
// computes sum of elements in a row
int sumRow( int** matrix, int row, int N)
{
    int sum = 0;
    //#pragma omp target parallel for reduction(+:sum)
    for (int i = 0; i < N; i++)
    {
        sum += matrix[row][i];
    }
    return sum;
}

// computes sum of elements in a column
int sumColumn( int** matrix, int col, int N)
{
    int sum = 0;
    //#pragma omp target teams distribute parallel for reduction(+:sum) map(to:matrix[:N][:N])
    for (int i = 0; i < N; i++)
    {
        sum += matrix[i][col];
    }
    return sum;
}


// checks if all elements in an array are equal
bool allEqual( int arr[], int N)
{
    for (int i = 0; i < N; i++){
        if (arr[0] != arr[i])
	{
            return false;
        }
    }
    return true;
}

bool isPairwiseDistinct( int** matrix, int N) {
    double s,e;
    s = omp_get_wtime();
    bool result = false;
    #pragma omp target teams distribute parallel map(to:matrix[:N][:N])
    {
        #pragma omp parallel for collapse(2) shared(result) schedule(static, CHUNK_SIZE)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int currentElement = matrix[i][j];

                #pragma omp parallel for collapse(2) reduction(||:result) schedule(static, CHUNK_SIZE)
                for (int row = 0; row < N; row++) {
                    for (int col = 0; col < N; col++) {
                        if (row != i || col != j) {
                            int otherElement = matrix[row][col];
                            if (currentElement == otherElement) {
                                {   
                                    result = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    e = omp_get_wtime();
    printf("isPairwiseDistinct: %.15f\n", e - s);
    return result;
    //#pragma omp declare target
    //bool found = false;
    //std::unordered_set<int> elementSet;
    //int i, j;
    //#pragma omp end declare target
    ////----------------------------------------------------------------
    //// OpenMP here!!!-------------------------------------------------
    //#pragma omp target teams distribute parallel map(to:matrix[:N][:N])
    //{   
    //    #pragma omp parallel for collapse(2) shared(found, elementSet) private(i, j) reduction(||:found)
    //    for (i = 0; i < N; i++) {
    //        for (j = 0; j < N; j++) {
    //            int currentElement = matrix[i][j];
//
    //            if (elementSet.find(currentElement) != elementSet.end()) {
    //                found = true;
    //            } else {
    //                elementSet.insert(currentElement);
    //            }
//
    //        }
    //    }
    //}
    //e = omp_get_wtime();
    //printf("isPairwiseDistinct: %.15f\n", e - s);
    //return found;
}

// checks if matrix is a magic square
bool isMagicSquare(int** matrix, int N)
{   

    int i;
    int row_sums[N];
    int col_sums[N];
    int main_diag_sum = 0;
    int anti_diag_sum = 0;
    double n1, n2, n3, n4, n5;
    n1 = omp_get_wtime();
    #pragma omp target teams distribute parallel map(to:matrix[:N][:N]) map(tofrom:row_sums[:N]) 
    {   
        if(omp_is_initial_device())
        {
          printf("Running on CPU\n");    
        }
        
        
        // compute row sums
        #pragma omp parallel for private(i) schedule(static, CHUNK_SIZE)
        for (i = 0; i < N; i++)
        {
            row_sums[i] = sumRow(matrix, i, N);
        }
    }
    if (!allEqual(row_sums, N)) return false;
    n2 = omp_get_wtime();
    #pragma omp target teams distribute parallel map(to:matrix[:N][:N]) map(tofrom:col_sums[:N])
    {   
        if(omp_is_initial_device())
        {
          printf("Running on CPU\n");    
        }
        // compute row sums
        #pragma omp parallel for private(i) schedule(static, CHUNK_SIZE)
        for (i = 0; i < N; i++)
        {
            col_sums[i] = sumColumn(matrix, i, N);
        }
    }
    if (!allEqual(col_sums, N)) return false;
    n3 = omp_get_wtime();
    //for (int i = 0; i < N; i++)
    //{
    //    row_sums[i] = sumRow(matrix, i, N);
    //}
    //if (!allEqual(row_sums, N)) return false;
    //// compute column sums
    //for (int i = 0; i < N; i++)
    //{
    //    col_sums[i] = sumColumn(matrix, i, N);
    //}
    //if (!allEqual(col_sums, N)) return false;

    #pragma omp target teams distribute parallel for reduction(+:main_diag_sum) map(to:matrix[:N][:N]) schedule(static, CHUNK_SIZE)
    // compute sum of elements on main diagonal
    for (int i = 0; i < N; i++)
    {
        main_diag_sum += matrix[i][i];
    }
    int row_sum = row_sums[0];
    if (main_diag_sum != row_sum) return false;
    n4 = omp_get_wtime();
    #pragma omp target teams distribute parallel for reduction(+:anti_diag_sum) map(to:matrix[:N][:N]) schedule(static, CHUNK_SIZE)
    // compute sum of elements on antidiagonal
    for (int i = 0; i < N; i++)
    {
        anti_diag_sum += matrix[i][N - 1 - i];
    }
    if (anti_diag_sum != row_sum) return false;
    n5 = omp_get_wtime();
    printf("Total sumrow: %.15f, total sumcol: %.15f, maindiag: %.15f, antidiag: %.15f\n", n2-n1,n3-n2,n4-n3,n5-n4);
    if(isPairwiseDistinct(matrix, N))
	    return false;
    return true;
}

int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Usage: %s <pattern_filename> <modifier_filename>\n", argv[0]);
        return 1;
    }

    // Timer Init
    double itime, ftime, gm;
    omp_set_nested(1);

    FILE *pattern_file = fopen(argv[1], "r");
    FILE *modifier_file = fopen(argv[2], "r");

    if (pattern_file == NULL) {
        printf("Error opening the pattern_file.\n");
        return 1;
    }

    if (modifier_file == NULL) {
        printf("Error opening the modifier_file.\n");
        return 1;
    }

    int N;
    if (fscanf(pattern_file, "%d", &N) != 1) {
        printf("Error reading the size of the matrix.\n");
        fclose(pattern_file);
        fclose(modifier_file);
        return 1;
    }

    if (fscanf(modifier_file, "%d", &N) != 1) {
        printf("Error reading the size of the matrix.\n");
        fclose(modifier_file);
        return 1;
    }

    // dynamically allocate memory for input and output matrices
    int** pattern = new int*[N];
    int** modifier = new int*[N];
    int M = N*N;

    int** magicSquare = new int*[M];
    for (int i = 0; i < M; i++) {
	    magicSquare[i] = new int[M];
    }

    // read-in matrix data
    for (int i = 0; i < N; i++) {
	    pattern[i] = new int[N];
	    modifier[i] = new int[N];
        for (int j = 0; j < N; j++) {
            if (fscanf(pattern_file, "%d", &pattern[i][j]) != 1) {
                printf("Error reading matrix values pattern.\n");
                fclose(pattern_file);
                return 1;
            }
            if (fscanf(modifier_file, "%d", &modifier[i][j]) != 1) {
                printf("Error reading matrix values modifier.\n");
                fclose(modifier_file);
                return 1;
            }
        }
    }

    fclose(pattern_file);
    fclose(modifier_file);

    //-------------------------------------//
    //BEGINNING-OF-COMPUTATION-------------//
    //-------------------------------------//

    // Timer Init
    itime = omp_get_wtime();

    generateMagicSquare(pattern, modifier, magicSquare, N, M);

    gm = omp_get_wtime();

    bool is_magic_square = isMagicSquare(magicSquare, M);
    //-------------------------------------//
    //BOOL FOR DETERMINING MAGIC SQUARE----//
    //-------------------------------------//

    // Timer end
    ftime = omp_get_wtime();

    // Timer print out
    printf("\n");
    printf("Generate: %.15f, isMS: %.15f\n", gm - itime, ftime - gm);
    printf("Total computation time: %.15f\n", ftime - itime);

    // Print first 3 and last 3 elements of generated and checked matrix 
    for (int i = 0; i < 3; i++)
    {
        printf("%d ", magicSquare[i][0]);
    }

    printf("... \n . \n . \n . \n");

    for (int j = M-3; j < M; j++)
    {
        printf("%d ", magicSquare[M-1][j]);
    }

    printf("\n");
    if(is_magic_square) printf("Generated matrix is a magic square.\n");
    else                printf("Generated matrix is not a magic square.\n");

    // CAN TRY TO PARALLEL
    // free dynamically allocated memory
    for (int i = 0; i < M; i++) {
        delete[] magicSquare[i];
    }
    delete[] magicSquare;

    for (int i = 0; i < N; i++) {
	    delete[] pattern[i];
	    delete[] modifier[i];
    }
    delete[] pattern;
    delete[] modifier;
}
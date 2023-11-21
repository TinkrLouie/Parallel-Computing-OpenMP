#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
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


void generateMagicSquare(int** pattern, int** modifier, int** magicSquare, int N, int M)
{   
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for collapse(2) schedule(guided)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
		    modifier[i][j] *= M;
	    }
    }
    // VERSION 1
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
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

    // VERSION 2
    //#pragma omp parallel for shared(pattern, modifier, magicSquare)
    //for (int i = 0; i < M; i++)
    //{
    //    int patternRow = i % N;
    //    int modifierRow = i / N;
    //    int* patternRowPtr = pattern[patternRow];
    //    int* modifierRowPtr = modifier[modifierRow];
    //    for (int j = 0; j < M; j++)
    //    {
    //        int patternCol = j % N;
    //        int modifierCol = j / N;
    //        magicSquare[i][j] = patternRowPtr[patternCol] + modifierRowPtr[modifierCol];
    //    }
    //}
    // VERSION 3
    int iOuter, jOuter;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for collapse(2) shared(magicSquare, pattern, modifier) private(iOuter, jOuter) schedule(guided)
    for (iOuter = 0; iOuter < M; iOuter += CHUNK_SIZE)
    {
        for (jOuter = 0; jOuter < M; jOuter += CHUNK_SIZE)
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
    // VERSION 4 --NOT WORKING
    //int body_start_index;
    //////----------------------------------------------------------------
    ////// OpenMP here!!!-------------------------------------------------
    //#pragma omp parallel private(body_start_index) shared(magicSquare, pattern, modifier)
    //for(body_start_index = 0; body_start_index < M - CHUNK_SIZE; body_start_index += CHUNK_SIZE) {
    //    int i;
    //    int body_end_index = body_start_index + CHUNK_SIZE;
    //    ////----------------------------------------------------------------
    //    //// OpenMP here!!!-------------------------------------------------
    //    #pragma omp for private(i) schedule(guided)
    //    #pragma unroll_and_jam (4)
    //    for(i = body_start_index; i < body_end_index; i++) {
    //        int j;
    //        int patternRow = i % N;
    //        int modifierRow = i / N;
    //        int* patternRowPtr = pattern[patternRow];
    //        int* modifierRowPtr = modifier[modifierRow];            
    //        for(j = body_start_index; j < body_end_index; j++) {
    //            int patternCol = j % N;
    //            int modifierCol = j / N;
    //            magicSquare[i][j] = patternRowPtr[patternCol] + modifierRowPtr[modifierCol];
    //        }
    //
    //    }
    //}
}
// computes sum of elements in a row
int sumRow( int** matrix, int row, int N)
{
    int sum = 0;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target parallel num_threads(32)
    #pragma omp parallel for reduction(+:sum) //schedule(guided)
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
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target parallel num_threads(32)
    #pragma omp parallel for reduction(+:sum) //schedule(guided)
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

//bool allEqual(int arr[], int N) {
//    bool result = true;
//
//    #pragma omp parallel for shared(result)
//    for (int i = 1; i < N; i++) {
//        // Use a critical section to update the result variable
//        #pragma omp critical
//        {
//            if (arr[0] != arr[i]) {
//                result = false;
//            }
//        }
//    }
//
//    return result;
//}

bool isPairwiseDistinct( int** matrix, int N) {
    bool found = false;
    std::unordered_set<int> elementSet;
    int i, j;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target parallel num_threads(32)
    #pragma omp parallel for collapse(2) shared(found, elementSet) private(i, j) schedule(guided)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            int currentElement = matrix[i][j];
            #pragma omp critical
            {
                if (elementSet.find(currentElement) != elementSet.end()) {
                    found = true;
                } else {
                    elementSet.insert(currentElement);
                }
            }
        }
    }
    return found;
}  
// checks if matrix is a magic square
bool isMagicSquare(int** matrix, int N)
{
    int row_sums[N];
    int col_sums[N];
    int main_diag_sum = 0;
    int anti_diag_sum = 0;
    int i;

    // compute row sums
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp target teams parallel private(i)
    {
        for (i = 0; i < N; i++)
        {   
            if(!omp_is_initial_device() && i == 0)
            {
              printf("Running on GPU\n");    
            }
            row_sums[i] = sumRow(matrix, i, N);
        }
    }
    if (!allEqual(row_sums, N)) return false;
    int row_sum = row_sums[0];

    // compute column sums
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for //schedule(guided)
    for (int i = 0; i < N; i++)
    {
        col_sums[i] = sumColumn(matrix, i, N);
    }
    if (!allEqual(col_sums, N)) return false;
    // compute sum of elements on main diagonal
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target parallel num_threads(32)
    #pragma omp parallel for reduction(+:main_diag_sum) //schedule(guided)
    for (int i = 0; i < N; i++)
    {
        main_diag_sum += matrix[i][i];
    }
    if (main_diag_sum != row_sum) return false;
    // compute sum of elements on antidiagonal
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target parallel num_threads(32)
    #pragma omp parallel for reduction(+:anti_diag_sum) //schedule(guided)
    for (int i = 0; i < N; i++)
    {
        anti_diag_sum += matrix[i][N - 1 - i];
    }
    if (anti_diag_sum != row_sum) return false;
    
    return !isPairwiseDistinct(matrix, N);
}
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <pattern_filename> <modifier_filename>\n", argv[0]);
        return 1;
    }
    // Timer Init
    double itime, ftime, exec_time;

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
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp parallel for
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

    //#pragma omp target parallel num_threads(32)
    //{
    //    if(omp_is_initial_device())
    //    {
    //      printf("Running on CPU\n");    
    //    }
    //    else{
    //      int num_teams= omp_get_num_teams(); 
    //      int num_threads_per_team = omp_get_num_threads();
    //      printf("Running on GPU with %d teams and %d threads per team\n", 
    //        num_teams, 
    //        num_threads_per_team
    //      );
    //    }
    //}
    //#pragma omp target teams parallel
    //{
    //    if(omp_is_initial_device())
    //    {
    //      printf("Running on CPU\n");    
    //    }
    //    else{
    //      int num_teams= omp_get_num_teams(); 
    //      int num_threads_per_team = omp_get_num_threads();
    //      printf("Running on GPU with %d teams and %d threads per team\n", 
    //        num_teams, 
    //        num_threads_per_team
    //      );
    //    }
    //}

    //-------------------------------------//
    //BEGINNING-OF-COMPUTATION-------------//
    //-------------------------------------//
    // Timer Init
    itime = omp_get_wtime();
    generateMagicSquare(pattern, modifier, magicSquare, N, M);
    bool is_magic_square = isMagicSquare(magicSquare, M);
    //-------------------------------------//
    //BOOL FOR DETERMINING MAGIC SQUARE----//
    //-------------------------------------//
    // Timer end
    ftime = omp_get_wtime();
    // Timer print out
    exec_time = ftime - itime;

    printf("Total computation time: %.15f\n", exec_time);
    printf("\n");
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
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp parallel for
    for (int i = 0; i < M; i++) {
        delete[] magicSquare[i];
    }
    delete[] magicSquare;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp parallel for
    for (int i = 0; i < N; i++) {
	    delete[] pattern[i];
	    delete[] modifier[i];
    }
    delete[] pattern;
    delete[] modifier;
}

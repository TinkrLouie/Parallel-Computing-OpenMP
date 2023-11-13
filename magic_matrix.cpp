#include <stdio.h>
#include <stdbool.h>
#include <omp.h>
#include <unordered_set>

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
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
		    modifier[i][j] *= M;
	    }
    }

    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int patternRow = i % N;
            int patternCol = j % N;
            magicSquare[i][j] = pattern[patternRow][patternCol];
	        magicSquare[i][j] += modifier[i/N][j/N];
        }
    }
}

// computes sum of elements in a row
int sumRow( int** matrix, int row, int N)
{
    int sum = 0;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp target teams distribute parallel for reduction(+:sum) //schedule(dynamic, 20)
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
    //#pragma omp target teams distribute parallel for reduction(+:sum) //schedule(dynamic, 20)
    for (int i = 0; i < N; i++)
    {
        sum += matrix[i][col];
    }
    return sum;
}

// checks if all elements in an array are equal
// TEST IF BETTER WITH PARALLEL OR NOT--------------------------------
// -------------------------------------------------------------------
//--------------------------------------------------------------------
bool allEqual( int arr[], int N)
{   
    bool found = true;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for
    for (int i = 0; i < N; i++){
        if (arr[0] != arr[i])
	    {
            found = false;
        }
    }
    return found;
}


// CAN PARALLEL
bool isPairwiseDistinct( int** matrix, int N) {
    bool found = false;
    std::unordered_set<int> elementSet;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int currentElement = matrix[i][j];
            if (elementSet.find(currentElement) != elementSet.end()) {
                found = true;
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

    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for reduction(+:main_diag_sum, anti_diag_sum)
    for (int i = 0; i < N; i++)
    {   
        // compute row sums
        row_sums[i] = sumRow(matrix, i, N);
        // compute column sums
        col_sums[i] = sumColumn(matrix, i, N);
         // compute sum of elements on main diagonal
        main_diag_sum += matrix[i][i];
        // compute sum of elements on antidiagonal
        anti_diag_sum += matrix[i][N - 1 - i];
    }
    if (!allEqual(row_sums, N)) return false;

    int row_sum = row_sums[0];

    if (!allEqual(col_sums, N)) return false;

    if (main_diag_sum != row_sum) return false;

    if (anti_diag_sum != row_sum) return false;
    
    if(isPairwiseDistinct(matrix, N))
	    return false;
    return true;
}

int main(int argc, char *argv[])
{

    // Timer Init
    double itime, ftime, exec_time;

    if (argc != 3) {
        printf("Usage: %s <pattern_filename> <modifier_filename>\n", argv[0]);
        return 1;
    }
    
    int d = 20;

    // Code for checking teams and threads num
    // int num_teams= omp_get_num_teams(); 
    // int num_threads_per_team = omp_get_num_threads();
    // printf("Running on GPU with %d teams and %d threads per team\n", 
    //   num_teams, 
    //   num_threads_per_team
    // );

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
    
    // CAN PARALLEL
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
	    magicSquare[i] = new int[M];
    }

    // read-in matrix data
    //int ret = 0;
    //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    //#pragma omp parallel for schedule(dynamic, d)
    for (int i = 0; i < N; i++) {
	    pattern[i] = new int[N];
	    modifier[i] = new int[N];
        for (int j = 0; j < N; j++) {
            if (fscanf(pattern_file, "%d", &pattern[i][j]) != 1) {
                printf("Error reading matrix values pattern.\n");
                fclose(pattern_file);
                return 1;
                //ret = 1;
            }
            if (fscanf(modifier_file, "%d", &modifier[i][j]) != 1) {
                printf("Error reading matrix values modifier.\n");
                fclose(modifier_file);
                return 1;
                //ret = 2;
            }
        }
    }

    //if (ret == 1) {
    //    printf("Error reading matrix values pattern.\n");
    //    return 1;
    //}
    //if (ret == 2) {
    //    printf("Error reading matrix values modifier.\n");
    //    return 1;
    //}

    fclose(pattern_file);
    fclose(modifier_file);

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

    // Timer print out
    exec_time = ftime - itime;
    printf("\n");
    printf("Total computation time: %.15f\n", exec_time);

    // CAN TRY TO PARALLEL
    // free dynamically allocated memory
     //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        delete[] magicSquare[i];
    }
    delete[] magicSquare;

     //----------------------------------------------------------------
    // OpenMP here!!!-------------------------------------------------
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
	    delete[] pattern[i];
	    delete[] modifier[i];
    }
    delete[] pattern;
    delete[] modifier;
}
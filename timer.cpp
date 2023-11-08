#include <stdio.h>
#include <omp.h>

// Function to perform some computation
void performComputation(int threadID) {
    // Simulate some computation
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }
    printf("Thread %d completed computation.\n", threadID);
}

int main() {
    const int num_threads = 4;  // Number of threads

    // Set the number of threads to use in parallel regions
    omp_set_num_threads(num_threads);

    // Start the timer
    double start_time = omp_get_wtime();

    // Parallel region
    #pragma omp parallel
    {
        int threadID = omp_get_thread_num();

        // Identify the start time for each thread
        double thread_start_time = omp_get_wtime();

        // Call the function to perform computation
        performComputation(threadID);

        // Calculate the elapsed time for this thread
        double thread_elapsed_time = omp_get_wtime() - thread_start_time;

        printf("Thread %d elapsed time: %f seconds\n", threadID, thread_elapsed_time);
    }

    // End the timer
    double end_time = omp_get_wtime();

    // Calculate the total elapsed time for all threads
    double total_elapsed_time = end_time - start_time;
    printf("Total elapsed time for all threads: %f seconds\n", total_elapsed_time);

    return 0;
}

// -------------
// TABULAR FORM
// -------------


#include <stdio.h>
#include <omp.h>

// Function to be measured
void compute_function_1() {
    // Your computation here
}

// Function to be measured
void compute_function_2() {
    // Your computation here
}

// Function to measure and report time
void measure_and_report(void (*func)(), const char* func_name) {
    double start_time = omp_get_wtime();
    func();  // Call the function to be measured
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    printf("%s: %.4f seconds\n", func_name, elapsed_time);
}

int main() {
    // Set the number of threads (adjust as needed)
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        #pragma omp single
        {
            measure_and_report(compute_function_1, "Function 1");
            measure_and_report(compute_function_2, "Function 2");
            // Add more functions to be measured as needed
        }
    }

    return 0;
}
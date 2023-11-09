// --- Exploring hierarchical parallelism with OpenMP ---
//
// Solve the following tasks to familiarize yourself with the 
// parallelization concept behind OpenMP by reading, executing and 
// modifying the below OpenMP program that illustrates how many teams and
// threads OpenMP has created.
//
// a) Read the program and analyse the purpose of the OpenMP directives and runtime routines it employs. 
//    You may refer to any OpenMP documentation, e.g., the quick reference guide available at 
//    https://www.openmp.org/wp-content/uploads/OpenMP-4.5-1115-CPP-web.pdf.
//    Which output do you expect the program to deliver?
// b) Compile and run the code on NCC:
//      1. Login to NCC: 
//          ssh your-user-name@ncc1.clients.dur.ac.uk 
//      2. Start interactive session on A100 GPU node via slurm command: 
//          srun -n 1 -c 2 --gres=gpu:1g.10gb:1 --partition=ug-gpu-small --pty /bin/bash
//      3. Load the Nvidia HPC SDK to make the OpenMP compiler available: 
//          module load nvidia-hpc
//      4. Compile the code with the nvc++ compiler:
//          nvc++ -fopenmp -mp=gpu task1.cpp -o xtask1
//      5. Run the code:
//          ./xtask1
// c) Modify the code such that it runs with multiple threads per team. By means of which clause can you control the number of threads per team? 
//    What happens if you do not specify this clause at all?
// d) Modify the code such that it runs with multiple teams. By means of which clause can you control the number of teams?
//    What happens if you do not specify this clause at all?
// e) Modify the code such that it runs with multiple teams *and* multiple threads per team. Does the order of the required directives matter?

#include <stdio.h>
#include <omp.h>

int main() 
{
  int num_devices = omp_get_num_devices();
  printf("Number of available target devices %d\n", num_devices);

  // c)
  #pragma omp target parallel num_threads(32)
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
  }

  // d)
  #pragma omp target teams num_teams(2)
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
  }

  // e)
  #pragma omp target teams parallel
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
  }
}

#!/bin/bash

#SBATCH --nodes=2                       # Number of nodes
#SBATCH --ntasks=2                      # Number of MPI ranks
#SBATCH --cpus-per-task=1               # Number of cores per MPI rank 
#SBATCH --partition=cpu           # number of compute nodes. 
#SBATCH -t 0:0:30       # time limit for job (format: days-hours:minutes:seconds) 
#SBATCH --ntasks-per-node=1             # How many tasks on each node

module load intel-oneapi/2022.1.2/mpi
mpicc task2_solution.c -o xtask2
mpirun -np 2 ./xtask2

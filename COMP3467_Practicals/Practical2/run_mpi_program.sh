#!/bin/bash

# Request resources:
#SBATCH -n 2           # number of MPI ranks (1 per CPU core)
#SBATCH -N 1           # number of compute nodes. 
#SBATCH --partition=cpu           # number of compute nodes. 
#SBATCH -t 0:0:30       # time limit for job (format: days-hours:minutes:seconds) 

module load intel-oneapi/2022.1.2/mpi
mpicc task1.c -o hello
mpirun ./hello

#!/bin/bash

# Request resources *per job step*, i.e., for each "srun" command:
#SBATCH -N 1                # number of compute nodes 
#SBATCH --cpus-per-task 1   # number of cores per task - relevant for OpenMP programs
#SBATCH --ntasks-per-node 4 # number of tasks per node - relevant for MPI programs
#SBATCH --partition=cpu     # slurm partition - existent partitions can be looked up via sinfo     
#SBATCH -t 00:06:00           # time limit for job (format: days-hours:minutes:seconds) 

#gather CPU information
lscpu

#load the MPI module that was active when the code was compiled
module load intel-oneapi/2022.1.2/mpi

make clean
./configure
make -j 4
cd testing

#slurm parameters are propagated to mpirun - that's why we don't need to specify -np here
mpirun ./xhpl

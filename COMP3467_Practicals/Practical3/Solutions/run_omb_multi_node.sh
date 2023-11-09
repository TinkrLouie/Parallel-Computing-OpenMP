#!/bin/bash

# Request resources *per job step*, i.e., for each "srun" command:
#SBATCH -N 2                # number of compute nodes 
#SBATCH --cpus-per-task 1   # number of cores per task - relevant for OpenMP programs
#SBATCH --ntasks-per-node 1 # number of tasks per node - relevant for MPI programs
#SBATCH --partition=cpu     # slurm partition - existent partitions can be looked up via sinfo     
#SBATCH -t 00:03:00           # time limit for job (format: days-hours:minutes:seconds) 

#load the MPI module that was active when the code was compiled
module load intel-oneapi/2022.1.2/mpi

make clean
./configure CC=/apps/intel/oneapi/2022.1.2/mpi/2021.5.1/bin/mpicc CXX=/apps/intel/oneapi/2022.1.2/mpi/2021.5.1/bin/mpicxx 
make
cd c/mpi/pt2pt/standard
#slurm parameters are propagated to mpirun - that's why we don't need to specify -np here
mpirun -np 2 ./osu_bw
mpirun -np 2 ./osu_latency

#!/bin/bash
#SBATCH -o ./Reports/magic_matrix_gpu_%j.out
#SBATCH -e ./Reports/magic_matrix_gpu_%j.err
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 2
#SBATCH -p ug-gpu-small
#SBATCH --time=00:05:00
#SBATCH --gres=gpu:1g.10gb:1
#SBATCH --exclusive

JOBID="magic_matrix_gpu"

# Load any necessary modules
module load nvidia-hpc

make clean && make

# Set the directory where the data sets are located
data_dir="./data_sets"

datasets=(3 10 20)

# Set OMP_NUM_THREADS to the number of available cores
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

# Loop over the datasets and submit jobs in parallel
for dataset in "${datasets[@]}"; do
    echo "---------------------------------------------"
    echo "Running magic_matrix_gpu.cpp with N="$dataset""
    echo -e
    # Set the input file paths
    pattern_file="$data_dir/pattern"$dataset"x"$dataset".dat"
    modifier_file="$data_dir/modifier"$dataset"x"$dataset".dat"

    ./"$JOBID" $pattern_file $modifier_file
    echo -e
    echo "---------------------------------------------"
done

exit 0
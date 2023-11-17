#!/bin/bash
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

SCRATCH_DIRECTORY=/${USER}/ACS/${SLURM_JOBID}
mkdir -p ${SCRATCH_DIRECTORY}
cd ${SCRATCH_DIRECTORY}

# Set the directory where the data sets are located
data_dir="./data_sets"

datasets=(3 10)

cp ${SLURM_SUBMIT_DIR}/magic_matrix_gpu ${SCRATCH_DIRECTORY}

# Set OMP_NUM_THREADS to the number of available cores
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK}

# Loop over the datasets and submit jobs in parallel
for dataset in "${datasets[@]}"; do
    # Set the input file paths
    pattern_file="$data_dir/pattern"$dataset"x"$dataset".dat"
    modifier_file="$data_dir/modifier"$dataset"x"$dataset".dat"

    ./"$JOBID" $pattern_file $modifier_file > my_output

    cp ${SCRATCH_DIRECTORY}/my_output ${SLURM_SUBMIT_DIR}
done

# Step out of the scratch directory and remove it
cd ${SLURM_SUBMIT_DIR}
rm -rf ${SCRATCH_DIRECTORY}

exit 0
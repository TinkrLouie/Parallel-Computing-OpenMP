SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL = /bin/bash
all:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)

load:
	module load nvidia-hpc
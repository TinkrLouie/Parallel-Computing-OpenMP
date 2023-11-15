SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu

all:
	module load nvidia-hpc
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)

install:
	module load nvidia-hpc
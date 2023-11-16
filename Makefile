SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu

all: build

build:
	module load nvidia-hpc \
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)

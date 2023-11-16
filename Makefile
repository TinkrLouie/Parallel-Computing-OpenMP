SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu

all: build

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

load:
	module load nvidia-hpc

clean:
	rm -rf $(OUT) 


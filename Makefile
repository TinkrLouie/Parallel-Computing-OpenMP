OBJS = magic_matrix.o
SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL:=/bin/bash

all:
	module load nvidia-hpc
	$(CC) $(FLAGS) $(SRC) -o $(OUT)
	#./magic_matrix_gpu ./data_sets/pattern20x20.dat ./data_sets/modifier20x20.dat

clean:
	rm -f $(OUT)
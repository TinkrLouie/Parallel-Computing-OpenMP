OBJS = magic_matrix.o
SRC	= magic_matrix.cpp
OUT	= magic_matrix
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL:=/bin/bash

all:
	module load nvidia-hpc
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
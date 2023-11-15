OBJS = magic_matrix.o
SRC	= magic_matrix.cpp
OUT	= magic_matrix
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL:=/bin/bash

all:
	module load nvidia-hpc
	$(CC) $(FLAGS) $(SRC) -o $(OUT)
	./magic_matrix ./data_sets/pattern20x20.dat ./data_sets/modifier20x20.dat

clean:
	rm -f $(OUT)
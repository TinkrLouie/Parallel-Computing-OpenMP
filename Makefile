OBJS = magic_matrix.o
SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL:=/bin/bash

all: build

clean:
	rm -rf build

module:
	module load nvidia-hpc

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

run_20:
	./magic_matrix_gpu ./data_sets/pattern20x20.dat ./data_sets/modifier20x20.dat

run_10:
	./magic_matrix_gpu ./data_sets/pattern10x10.dat ./data_sets/modifier10x10.dat

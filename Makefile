SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL := /bin/bash
MOD = nvidia-hpc

all: build

build: 
	$(shell module load $(MOD) && \
	$(CC) $(FLAGS) $(SRC) -o $(OUT))

.PHONY: all clean

clean:
	rm -rf $(OUT)
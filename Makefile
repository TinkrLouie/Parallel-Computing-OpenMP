SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL := /bin/bash
MOD = nvidia-hpc
RMS = run_all_magic_matrix.sh

all: build

build: 
	$(shell module load $(MOD) && \
	$(CC) $(FLAGS) $(SRC) -o $(OUT))

clean:
	rm -rf $(OUT)
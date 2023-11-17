SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL := /bin/bash
MOD = nvidia-hpc

all: $(OUT)

$(OUT): $(SRC) 
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

load:
	$(shell module load $(MOD))

.PHONY: all clean

clean:
	rm -rf $(OUT)
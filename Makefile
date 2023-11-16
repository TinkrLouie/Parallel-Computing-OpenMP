SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
#MODULE_SCRIPT = load_modules.sh
#SHELL := !/bin/bash

all: build

load:
	. $(MODULE_SCRIPT)

build:
	module load nvidia-hpc; \
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)

SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
MODULE_SCRIPT = load_modules.sh
SHELL := !/bin/bash

all: load build

load:
	source $(MODULE_SCRIPT)

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)
	
clean:
	rm -rf $(OUT)

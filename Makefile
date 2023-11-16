SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
LOAD = load.sh
SH = /bin/bash

all: build

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

load:
	source ./$(LOAD)

clean:
	rm -rf $(OUT) 


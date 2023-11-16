SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
LOAD = load.sh
SHELL = /bin/bash
MOD = nvidia-hpc

all: build

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

load:
	chmod +x ./$(LOAD) && \
	./$(LOAD)

clean:
	rm -rf $(OUT) 


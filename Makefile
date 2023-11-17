SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL := /bin/bash
MOD := nvidia-hpc

all: $(OUT)

$(OUT): $(SRC)
	$(shell module load $(MOD) && \
	$(CC) $(FLAGS) $(SRC) -o $(OUT))

.PHONY: clean

clean:
	rm -rf $(OUT)
SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
LOAD = load.sh
SHELL := /bin/bash
MOD = nvidia-hpc
RMS = run_all_magic_matrix.sh
NM = install_numact1.sh

all: build

build: $(OUT)
	$(shell module load $(MOD) && $(CC) $(FLAGS) $(SRC) -o $(OUT))

clean:
	rm -rf $(OUT)
SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
LOAD = load.sh
SHELL = /bin/bash
MOD = nvidia-hpc
RMS = run_all_magic_matrix.sh
NM = install_numact1.sh

all: build

build:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

load:
	chmod +x ./$(LOAD) && \
	./$(LOAD)

clean:
	rm -rf $(OUT) $(NM) $(RMS)


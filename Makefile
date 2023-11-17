SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
SHELL := /bin/bash
MOD := nvidia-hpc

default: help

.PHONY: help
help: # Show help for Makefile recipes
	@grep -E '^[a-zA-Z0-9 -]+:.*#'  Makefile | sort | while read -r l; do printf "\033[1;32m$$(echo $$l | cut -f 1 -d':')\033[00m:$$(echo $$l | cut -f 2- -d'#')\n"; done

.PHONY: mmgpu
mmgpu: $(OUT) # Build the code

$(OUT): $(SRC)
	module load $(MOD) && \
	$(CC) $(FLAGS) -c $^ -o $@.o && \
	$(CC) $(FLAGS) $@.o -o $@

.PHONY: clean
clean: # Cleanup
	rm -rf $(OUT) $(OUT).o
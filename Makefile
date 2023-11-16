SRC	= magic_matrix_gpu.cpp
OUT	= magic_matrix_gpu
CC = nvc++
FLAGS = -fopenmp -mp=gpu
MODULE_SCRIPT = load_modules.sh

all:
	source $(MODULE_SCRIPT) && $(CC) $(FLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT)

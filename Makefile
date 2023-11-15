OBJS	= magic_matrix.o
SRC	= magic_matrix.cpp
OUT	= magic_matrix
CC	 = nvc++
FLAGS	 = -fopenmp -mp=gpu

all:
	module load nvidia-hpc
	$(CC) $(FLAGS) $(OUT) -o $(SRC)

clean:
	rm -f $(OUT)
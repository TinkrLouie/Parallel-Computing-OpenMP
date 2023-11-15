OBJS	= magic_matrix.o
SRC	= magic_matrix.cpp
OUT	= magic_matrix
CC	 = nvc++
FLAGS	 = -fopenmp -mp=gpu

all: $(OUT)
	$(CC) $(FLAGS) $(OUT) -o $(SRC)

clean:
	rm -f $(OUT)
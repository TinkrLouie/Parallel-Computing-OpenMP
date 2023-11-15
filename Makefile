OBJS	= magic_matrix.o
SOURCE	= magic_matrix.cpp
OUT	= magic_matrix
CC	 = nvc++
FLAGS	 = -fopenmp -mp=gpu

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

magic_matrix.o: magic_matrix.cpp
	$(CC) $(FLAGS) magic_matrix.cpp 


clean:
	rm -f $(OBJS) $(OUT)
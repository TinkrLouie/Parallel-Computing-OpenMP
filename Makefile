CC = nvc++
CFLAGS = -fopenmp -mp=gpu
TARGET = simple

all: $(TARGET)

$(TARGET): simple.cpp
    $(CC) $(CFLAGS) $(TARGET) -o simple.cpp

clean:
    rm -rf *$(TARGET)


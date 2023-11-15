CC = nvc++
CFLAGS = -fopenmp -mp=gpu
TARGET = simple

all: $(TARGET)

$(TARGET): simple.cpp
    $(CC) $(CFLAGS) $^ -o $@

clean:
    rm -f $(TARGET)
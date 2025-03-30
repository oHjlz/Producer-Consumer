CC=gcc
CFLAGS=-Wall -pthread   # Adding pthread support for threads
OBJ=buffer.o            # You can add other object files here if needed

# Default target that will be executed when running 'make'
all: buffer

# Target for linking the object files into the final executable
buffer: buffer.o
	$(CC) $(CFLAGS) -o buffer buffer.o

# Rule for compiling the source file into an object file
buffer.o: buffer.c
	$(CC) $(CFLAGS) -c buffer.c

# Clean rule to remove object files and the executable
clean:
	rm -rf *.o buffer

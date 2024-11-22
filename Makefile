# Compiler and flags
CC = gcc
CFLAGS = -Wall -lpthread -lpcap

# Source files
SOURCES = main.c capture.c ddos.c mitm.c

# Object files (compiled source files)
OBJECTS = $(SOURCES:.c=.o)

# Output executable
EXEC = my_program

# Default target (to build the program)
all: $(EXEC)

# Rule to link object files into an executable
$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS) $(CFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJECTS) $(EXEC)

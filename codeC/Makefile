# Name of the executable
TARGET = exeC

# Source files
SRCS = main.c avltree_utils.c avltree_usage.c utils.c

# Object files (generated from source files)
OBJS = $(SRCS:.c=.o)

# Compiler
CC = gcc

# Compilation options
CFLAGS = -Wall -Wextra -Werror -std=c11

# Main rule: Compile the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f *.o $(TARGET)

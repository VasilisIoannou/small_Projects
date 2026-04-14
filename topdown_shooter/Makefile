# 1. Variables: Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = game

# 2. List of source files
# This will automatically find all .c files in the current folder
SRCS = $(wildcard *.c)

# 3. List of object files
# This turns the list of .c files into .o files
OBJS = $(SRCS:.c=.o)

# 4. The default rule (what happens when you just type 'make')
all: $(TARGET)

# 5. Rule to link the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 6. Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 7. Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJS)

# 1. Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
# Link the math library (-lm)
LIBS = -lm

TARGET1 = imageBundle.exe
TARGET2 = image_to_terminal.exe

COMMON_SRCS = fileImage.c image.c screen.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

all: $(TARGET1) $(TARGET2)

$(TARGET1): imageBundle.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET1) imageBundle.o $(COMMON_OBJS) $(LIBS)

$(TARGET2): image_to_terminal.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET2) image_to_terminal.o $(COMMON_OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET1) $(TARGET2) *.o

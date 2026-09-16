TARGET = cw

INCDIR = ./include

CC = gcc

CFLAGS = -I$(INCDIR) -Wall -Wextra -std=c11 $(shell pkg-config --cflags libpng)
LDFLAGS = $(shell pkg-config --libs libpng) -lm

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
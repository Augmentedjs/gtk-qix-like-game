# Makefile for the QIX-style line drawing program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = $(shell pkg-config --cflags gtk4)

# Linker flags
LDFLAGS = $(shell pkg-config --libs gtk4)

# Source files
SRCS = qix_line.c

# Output executable
TARGET = qix_line

# Build the target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)

.PHONY: all clean

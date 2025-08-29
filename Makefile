# Makefile para Tic-Tac-Toe con raylib en MSYS2 MinGW64
CC := gcc
CFLAGS := -Wall -Wextra -O2
LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm

TARGET := tateti
SRC := tateti.c
OBJ := $(SRC:.c=.o)

.PHONY: all run clean rebuild

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET).exe

clean:
	rm -f $(OBJ) $(TARGET).exe

rebuild: clean all


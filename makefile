CC = gcc
CFLAGS = -Iinclude
SRC = src/main.c src/finance_engine.c
OBJ = finance_engine

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) sorted_export.csv
CC = gcc
SRC = ./main.c

all: main

main: $(SRC)
	$(CC) $(SRC) -o ./bin/main -w
	./bin/main

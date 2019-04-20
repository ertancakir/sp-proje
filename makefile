INC = ./include/libfdr
LIB = ./include/libfdr
CC = gcc
SRC = ./src/main.c

all: main

main: $(SRC)
	$(CC) -I$(INC) $(SRC) -o ./bin/main -L$(LIB) -lfdr -w
	./bin/main

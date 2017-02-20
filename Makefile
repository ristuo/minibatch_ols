IDIR=include
SRCDIR=src
CC=gcc
OPTS = -Wall -Wextra -pedantic
VPATH=$(SRCDIR):$(IDIR)

main: main.c io.c matrix.c dataset.c train.c model.c util.c
	$(CC) $(OPTS) -o main $^

.PHONY: grind 

grind: main
	valgrind --track-origins=yes --leak-check=full ./main

.PHONY: run
run: main
	./main

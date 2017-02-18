IDIR=include
SRCDIR=src
CC=gcc
OPTS = -Wall -Wextra -pedantic
VPATH=$(SRCDIR):$(IDIR)

main: main.c io.c matrix.c dataset.c
	$(CC) $(OPTS) -o main $^

.PHONY: run

run: main
	./main

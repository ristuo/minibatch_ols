IDIR=include
SRCDIR=src
CC=gcc
OPTS = -Wall -Wextra -pedantic -D DEBUG
VPATH=$(SRCDIR):$(IDIR)

main: main.c io.c matrix.c dataset.c train.c model.c
	$(CC) $(OPTS) -o main $^

.PHONY: run

run: main
	./main

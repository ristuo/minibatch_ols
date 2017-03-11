IDIR=include
SRCDIR=src
CC=gcc
OPTS = -Wall -Wextra -pedantic
VPATH=$(SRCDIR):$(IDIR)
LIBS = -lpthread

main: main.c io.c matrix.c dataset.c train.c model.c util.c
	$(CC) $(OPTS) -o main $^ $(LIBS)

.PHONY: grind 

grind: main
	valgrind --track-origins=yes --leak-check=full ./main

.PHONY: run
run: main
	./main

.PHONY: time
time: main
	time ./main > target/res.csv

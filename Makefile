IDIR=../include
SRCDIR=../src
CC=gcc
OPTS = -Wall -Wextra -pedantic
VPATH = ../src:../include

csvread: csvread.o
	
csvread.o: csvread.c matrix.c
	$(CC) $(OPTS) -o csvread $^

.PHONY: run

run: csvread
	./csvread ../data/testy.csv

CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g

all: main

main: main.c interval_tree.c interval_tree.h
	$(CC) $(CFLAGS) -o main main.c interval_tree.c

run: all
	./main

clean:
	rm main


.PHONY: all run clean

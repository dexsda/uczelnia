CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: zad1a

zad1a: zad1.c
	$(CC) $(CFLAGS) -D FORK -o $@ $<

clean:
	rm zad1a

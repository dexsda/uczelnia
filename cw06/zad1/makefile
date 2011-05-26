CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: zad1-sleep zad1-nosleep

zad1-sleep: zad1.c
	$(CC) $(CFLAGS) -D SLEEP  -o $@ $<

zad1-nosleep: zad1.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm zad1-sleep zad1-nosleep

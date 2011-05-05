CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: sender receiver

sender: sender.c
	$(CC) $(CFLAGS) -o $@ $<

receiver: receiver.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm zad1

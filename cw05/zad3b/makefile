CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: child parent

child: child.c
	$(CC) $(CFLAGS) -o $@ $<

parent: parent.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm child parent

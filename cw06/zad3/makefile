CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: zad3-prog

zad3-prog: prog.c
	$(CC) $(CFLAGS) -o $@ $<

cycle: zad3-prog
	read | { ./zad3-prog | ./zad3-prog | ./zad3-prog | ./zad3-prog | ./zad3-prog; } >> /dev/stdin

clean:
	rm zad3-prog zad3-pipes

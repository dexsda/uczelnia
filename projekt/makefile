CC=gcc
FLAGS=-Wall -O2 
CRAP=-lpthread

all: daemon shell

daemon: daemon.c
	$(CC) $(FLAGS) $(CRAP) $^ -o $@

shell: shell.c
	$(CC) $(FLAGS) $(CRAP) $^ -o $@

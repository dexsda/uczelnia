CC=gcc
FLAGS=-Wall -O2 -lpthread

all: serv cli

serv: serv.c
	$(CC) $(FLAGS) $^ -o $@

cli: cli.c
	$(CC) $(FLAGS) $^ -o $@

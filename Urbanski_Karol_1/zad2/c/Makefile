CC = gcc
SRC=server.c
CFLAGS=-Wall

all: server

server: $(SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf server

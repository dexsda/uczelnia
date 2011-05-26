CC = gcc
CFLAGS = -Wall -O2
.phony: clean

all: client server

client: client.c
	$(CC) $(CFLAGS) -o $@ $<

server: server.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm client server

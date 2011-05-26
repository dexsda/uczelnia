CC=gcc
FLAGS=-Wall -O2

all: client server

client: client.c
		$(CC) $(CFLAGS) client.c -o client -lrt

server: server.c
		$(CC) $(CFLAGS) server.c -o server -lrt

clean: 
	rm client server

CC=gcc
FLAGS=-Wall -O2

all: client server

client: client.c
		$(CC) $(CFLAGS) client.c -o client

server: server.c
		$(CC) $(CFLAGS) server.c -o server

clean: 
	rm client server

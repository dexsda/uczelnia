CC=gcc
CCFLAGS=-Wall -lrt -pthread
.PHONY : all clean

all : pisarz czytelnik alloc

pisarz : pisarz.c
	$(CC) $(CCFLAGS) $^ -o $@

czytelnik : czytelnik.c
	$(CC) $(CCFLAGS) $^ -o $@

alloc : alloc.c
	$(CC) $(CCFLAGS) $^ -o $@

clean :
	rm -f pisarz czytelnik alloc


CC=gcc
CCFLAGS=-Wall -O2
.PHONY : all clean

all : konsument producent alloc

konsument : konsument.c
	$(CC) $(CCFLAGS) $^ -o $@

producent : producent.c
	$(CC) $(CCFLAGS) $^ -o $@

alloc : alloc.c
	$(CC) $(CCFLAGS) $^ -o $@

clean :
	rm -f konsument producent alloc


CC=gcc
CCFLAGS=-Wall -O2
.PHONY : all clean

all : var1

var1 : var1.c
	$(CC) $(CCFLAGS) $^ -o $@ -lpthread

clean :
	rm -f var1


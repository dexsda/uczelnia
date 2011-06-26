CC=gcc
CCFLAGS=-Wall -O2
.PHONY : all clean

all : var1 var2

var1 : var1.c
	$(CC) $(CCFLAGS) $^ -o $@ -lpthread

var2 : var2.c
	$(CC) $(CCFLAGS) $^ -o $@ -lpthread

clean :
	rm -f var1 var2


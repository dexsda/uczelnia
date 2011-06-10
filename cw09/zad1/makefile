CC=gcc
CCFLAGS=-Wall -O2
.PHONY : all clean

all : wczytanie1 wczytanie2 wczytanie3

wczytanie1 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 $^ -o $@ -lpthread

wczytanie2 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER2 $^ -o $@ -lpthread

wczytanie3 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER3 $^ -o $@ -lpthread

clean :
	rm -f wczytanie1 wczytanie2 wczytanie3


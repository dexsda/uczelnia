CC=gcc
CCFLAGS=-Wall -O2
.PHONY : all clean

all : wczytanie_1 wczytanie_2 wczytanie_3 wczytanie_4 wczytanie_5 wczytanie_6

wczytanie_1 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG1 $^ -o $@ -lpthread

wczytanie_2 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG2 $^ -o $@ -lpthread

wczytanie_3 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG3 $^ -o $@ -lpthread

wczytanie_4 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG4 $^ -o $@ -lpthread

wczytanie_5 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG5 $^ -o $@ -lpthread

wczytanie_6 : wczytanie.c
	$(CC) $(CCFLAGS) -D VER1 -D SIG6 $^ -o $@ -lpthread

clean :
	rm -f wczytanie


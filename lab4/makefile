CC = gcc
CFLAGS = -Wall
INCLUDE = -I/usr/local/include
LIB = -L/usr/local/lib -lm -lgsl -lgslcblas

ifeq ($(F),kwadr)
       FUNKCJA=-D KWADRATOWE
endif
ifeq ($(F),tryg)
       FUNKCJA=-D TRYGONOMETRYCZNE
endif
ifeq ($(F),czwar)
       FUNKCJA=-D CZWARTY_STOPIEN
endif

all: bisection falsepos brent newton secant steffenson

bisection: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D BRACKETING -D BISECTION $(INCLUDE) $(LIB) -o $@
	
falsepos: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D BRACKETING -D FALSEPOS $(INCLUDE) $(LIB) -o $@
	
brent: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D BRACKETING -D BRENT $(INCLUDE) $(LIB) -o $@
	
newton: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D POLISHING -D NEWTON $(INCLUDE) $(LIB) -o $@

secant: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D POLISHING -D SECANT $(INCLUDE) $(LIB) -o $@

steffenson: zad3.c
	$(CC) $(CFLAGS) $^ $(FUNKCJA) -D POLISHING -D STEFFENSON $(INCLUDE) $(LIB) -o $@

clean:
	rm -f bisection falsepos brent newton secant steffenson

.PHONY: clean

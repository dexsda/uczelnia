CC = gcc
CFLAGS = -Wall
LDFLAGS = -O2 -lm
PROGRAM = calc

all: ${PROGRAM}

calc: calc.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean: 
	rm -f ${PROGRAM}



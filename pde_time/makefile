CC = gcc
CFLAGS = -Wall
LDFLAGS = -O2 -lm
PROGRAM = exp imp

all: ${PROGRAM}

exp: exp.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

imp: imp.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean: 
	rm -f ${PROGRAM}



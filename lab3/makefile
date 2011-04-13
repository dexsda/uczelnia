CC    = gcc
CFLAGS   = -ansi -pedantic -Wall
LDFLAGS = -O2 -I/usr/tools/include -L/usr/tools/lib -lm -lgsl -lgslcblas
PROGRAM   = integrate

all: ${PROGRAM}

integrate: gsl_integration.c
	${CC} ${CFLAGS} ${LDFLAGS} -o integrate gsl_integration.c

clean: 
	rm -f ${PROGRAM}



CC    = gcc
CFLAGS   = -ansi -pedantic -Wall
LDFLAGS = -O2 -I/usr/tools/include -L/usr/tools/lib -lm -lgsl -lgslcblas
PROGRAM   = integrate

all: ${PROGRAM}

integrate: gsl_integration.c
	${CC} ${CFLAGS} ${LDFLAGS} -o integrate gsl_integration.c

test:
	@./integrate 0.1
	@./integrate 0.05
	@./integrate 0.01
	@./integrate 0.001
	@./integrate 0.0001
	@./integrate 0.000001
	@./integrate 0.0000001
clean: 
	rm -f ${PROGRAM}



CC = gcc
CFLAGS = -Wall -O2
TEST_1=16384
TEST_2=65536
TEST_3=131072
TEST_4=262144
.phony: clean

all: zad1a zad1b zad1c zad1d

zad1a: zad1.c
	$(CC) $(CFLAGS) -D FORK -o $@ $<
zad1b: zad1.c
	$(CC) $(CFLAGS) -D VFORK -o $@ $<
zad1c: zad1.c
	$(CC) $(CFLAGS) -D CLONE -o $@ $<
zad1d: zad1.c
	$(CC) $(CFLAGS) -D VCLONE -o $@ $<

test: zad1a zad1b zad1c zad1d
	@./zad1a ${TEST_1} && ./zad1b ${TEST_1} && ./zad1c ${TEST_1} && ./zad1d ${TEST_1}
	@./zad1a ${TEST_2} && ./zad1b ${TEST_2} && ./zad1c ${TEST_2} && ./zad1d ${TEST_2}
	@./zad1a ${TEST_3} && ./zad1b ${TEST_3} && ./zad1c ${TEST_3} && ./zad1d ${TEST_3}
	@./zad1a ${TEST_4} && ./zad1b ${TEST_4} && ./zad1c ${TEST_4} && ./zad1d ${TEST_4}
clean:
	rm zad1a zad1b zad1c zad1d

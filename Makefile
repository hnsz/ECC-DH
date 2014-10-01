CC=gcc
FLAGS=-Wall -pedantic --std=c99
CFLAGS=$(FLAGS) -c
OBJS=ecc_mul.o


all: a.out

a.out: main.c $(OBJS)
	$(CC) $(FLAGS) main.c $(OBJS)

ecc_mul.o: ecc_mul.c ecc_mul.h
	$(CC) $(CFLAGS) ecc_mul.c

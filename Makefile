CC=gcc
FLAGS=-Wall -pedantic --std=c99
CFLAGS=$(FLAGS) -c
OBJS=mul_by_add.o ecc.o



a.out: main.c $(OBJS)
	$(CC) $(FLAGS) main.c $(OBJS) -lcrypto

mul_by_add.o: mul_by_add.c mul_by_add.h
	$(CC) $(CFLAGS) mul_by_add.c

ecc.o: ecc.c ecc.h
	$(CC) $(CFLAGS) ecc.c 


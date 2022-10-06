#Makefile

CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra -Werror



test: expr_test


#main
expr_test:expr_test.o expr.o
	$(CC) $(CFLAGS) expr_test.o expr.o -o expr_test

#.o
expr.o: expr.c expr.h
	$(CC) -c expr.c

expr_test.o: expr_test.c
	$(CC) -c expr_test.c


#cleaning
clean:
	rm -f *.o expr_test
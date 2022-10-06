#Makefile

CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g#-Werror

SCANNER_TEST=tests/scan_test.c src/scanner.c src/scanner.h

scanner_test: $(SCANNER_TEST)
	$(CC) $(CFLAGS) -o scan_test $(SCANNER_TEST)

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

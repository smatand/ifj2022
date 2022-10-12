#Makefile

CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra -Werror -g



test: expr_test


#main
expr_test: expr_test.o expr.o expr_stack.o
	$(CC) $(CFLAGS) expr_test.o expr.o expr_stack.o -o expr_test

# expr: expr.o expr_stack.o
# 	$(CC) $(CFLAG) expr.o expr_stack.o -o expr
#.o
# scanenr.o: scanner.c scanner.h
# 	$(CC) -c scanner.c

expr.o: expr.c expr.h
	$(CC) -c expr.c

expr_test.o: expr_test.c
	$(CC) -c expr_test.c

expr_stack.o: expr_stack.c expr_stack.h
	$(CC) -c expr_stack.c


#cleaning
clean:
	rm -f *.o expr_test
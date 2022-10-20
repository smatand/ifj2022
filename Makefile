#Makefile

CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g
SRC=src
SCANNER_TEST=tests/scan_test.c src/scanner.c src/scanner.h src/str.h src/str.c
EXPRESSION= $(SRC)/expr.o $(SRC)/expr_stack.o $(SRC)/scanner.o $(SRC)/str.o
.PHONY: scanner test main clean


scanner_test: $(SCANNER_TEST)
	$(CC) $(CFLAGS) -o scan_test $(SCANNER_TEST)

test: expr_test
main: expr

#main
expr_test: expr_test.o expr.o expr_stack.o
	$(CC) $(CFLAGS) $^ -o $(SRC)/$^

expr: $(EXPRESSION)
	$(CC) $(CFLAGS) $^ -o $(SRC)/$@

#.o

%.o:$(SRC)/%.c
	$(CC) -c -o $(SRC)/$@ $^

clean:
	rm -f $(SRC)/*.o $(SRC)/expr_test src/expr

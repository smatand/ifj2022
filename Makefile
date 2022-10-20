#Makefile
CC=gcc
SRC=src
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g 
SCANNER_TEST=tests/scan_test.c src/scanner.c src/scanner.h src/str.h src/str.c
.PHONY: scanner_test clean



scanner_test: $(SCANNER_TEST)
	$(CC) $(CFLAGS) -o scan_test $(SCANNER_TEST)



clean:
	rm -f $(SRC)/*.o

#Makefile

CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g

.PHONY: test main clean

compiler: ifj22

################################################################### EXECUTABLES ###

ifj22: main.o scanner.o str.o parser.o syntax_rules.o token.o symtable.o sym_table_stack.o generator.o expr.o expr_stack.o dll.o
	$(CC) $(CFLAGS) $^ -o $@

################################################################## OBJECT FILES ###

main.o: main.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

scanner.o: scanner.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

str.o: str.c str.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c parser.h symtable.h scanner.h error.h sym_table_stack.h generator.h dll.h
	$(CC) $(CFLAGS) -c $< -o $@

syntax_rules.o: syntax_rules.c parser.h parser_macros.h error.h sym_table_stack.h dll.h generator.h
	$(CC) $(CFLAGS) -c $< -o $@

token.o: token.c token.h symtable.h error.h str.h
	$(CC) $(CFLAGS) -c $< -o $@

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c $< -o $@

sym_table_stack.o: sym_table_stack.c sym_table_stack.h symtable.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

generator.o: generator.c generator.h dll.h error.h parser.h dll.h
	$(CC) $(CFLAGS) -c $< -o $@

expr.o: expr.c expr.h scanner.h str.h error.h expr_stack.h
	$(CC) $(CFLAGS) -c $< -o $@

expr_stack.o: expr_stack.c expr_stack.h scanner.h expr.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

dll.o: dll.c dll.h
	$(CC) $(CFLAGS) -c $< -o $@

scan_test.o: scan_test.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -c $< -o $@

###################################################################################


e: expr

#main
expr_test: expr_test.o expr.o expr_stack.o
	$(CC) $(CFLAGS) $^ -o $@

scan_test: scan_test.o scanner.o str.o
	$(CC) $(CFLAGS) $^ -o $@
	mv scan_test ../tests
	cd ../tests/ && ./scanner_test.sh
	#../tests/scanner_test.sh
student_test: ifj22
	git submodule update --remote --merge
	cd ../IFJ22_Tester && python3 test.py ../src/ifj22 ../interpret/ic22int

expr: expr.o expr_stack.o scanner.o str.o generator.o dll.o parser.o token.o symtable.o sym_table_stack.o
	$(CC) -o $@ $^

clean:
	rm -f *.o *.d  expr ifj22

#Makefile

CC=gcc
# CODEDIRS=.
# DEPFLAGS= -MP -MD -MM
CFLAGS=-std=c11 -pedantic -Wall -Wextra -g
##auto dependenci for H files not working
# CFLAGS=-std=c11 -pedantic -Wall -Wextra -g $(foreach D,$(CODEDIRS),-I$(D)) $(DEPFLAGS)
# CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.c))
# OBJECTS=$(patsubst %.c,%.o,$(CFILES))
# DEPFILES=$(patsubst %.c,%.d,$(CFILES))
EXPRESSION= expr.o expr_stack.o scanner.o str.o
.PHONY: test main clean



test: expr_test
main: expr
h:
	make clean
	make expr


#main
expr_test: expr_test.o expr.o expr_stack.o
	$(CC) $(CFLAGS) $^ -o $@

expr: $(EXPRESSION)
	$(CC) -o $@ $^

#.o

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.d expr_test expr 

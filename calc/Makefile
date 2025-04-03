CFLAGS = -Wall -Wextra -O3

.PHONY: clean generate

all: calc

calc.c: calc.y
	bison -o calc.c calc.y

calc: calc.c
	clang $(CFLAGS) -o $@ calc.c -lm

clean:
	rm -f calc calc.c
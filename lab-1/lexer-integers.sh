#!/bin/sh

SIGN="[+-]?"
INTEGER_LITERALS="(u|U|l|L|ll|LL|ul|UL|uL|Ul|ull|ULL|uLL|Ull|llu|LLu|llU|LLU|z|Z|uz|UZ|uZ|Uz)?"

DEC="0|[1-9]('?[0-9]+)*"
OCT="0[0-7]('?[0-7]+)*"
HEX="0[xX]('?[0-9a-fA-F]+)*"
BINARY="0[bB]('?[01]+)*"
INTEGER="$SIGN($DEC|$OCT|$HEX|$BINARY)$INTEGER_LITERALS"

FILE="test.cpp"

grep -n -o -E "$INTEGER" $FILE
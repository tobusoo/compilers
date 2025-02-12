#!/bin/sh

FILE="test.cpp"
IDENT="[_a-zA-Z][_a-zA-Z0-9]*"
REGEXP="[*&]?$IDENT(\[$IDENT\]|\.\*?$IDENT|->\*?$IDENT)|[*&]$IDENT"

grep -n -o -E "$REGEXP" $FILE
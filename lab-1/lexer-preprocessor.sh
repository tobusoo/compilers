#!/bin/sh

FILE="test.cpp"

SPACE="[ \t]*"
REGEXP="$SPACE[#]$SPACE[a-z]{2,}($SPACE.*)*"
grep -n -o -E "$REGEXP" $FILE
#!/bin/sh

FILE="test.cpp"
REGEXP="[_a-zA-Z][_a-zA-Z0-9]*"
grep -n -o -E "$REGEXP" $FILE
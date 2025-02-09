#!/bin/sh

FILE="test.cpp"
# TODO
REGEXP="[_a-zA-Z]\w+"
grep -n -o -E "$REGEXP" $FILE
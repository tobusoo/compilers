#!/bin/sh

FILE="test.cpp"

REGEX=" (==|!=|<|>|<=|>=|<=>) "
grep -n -o -E "$REGEX" $FILE
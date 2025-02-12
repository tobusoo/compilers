#!/bin/sh

REGEX="(u8|u|U|L)?'(\\\.+|.)'"
FILE="test.cpp"

grep -n -o -E "$REGEX" $FILE
#!/bin/sh

FILE="test.cpp"

ONELINE="//.+"
MULTILINE="\\/\\*(\\s*|.)*\\*\\/"
REGEXP="$ONELINE|$MULTILINE"

grep -Pzo "$REGEXP" $FILE

#!/bin/sh

FILE="test.cpp"

RAW_STRINGS="(R|LR|u8R|uR|UR)\"\([^\"]*\)\""
STRINGS="(L|u8|u|U)?\"([^\"])*\""

REGEX="$RAW_STRINGS|$STRINGS"
grep -n -o -E "$REGEX" $FILE
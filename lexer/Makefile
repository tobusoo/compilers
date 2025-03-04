CXXFLAGS = -Wall -Wextra -std=c++20 -O3

.PHONY: clean generate

all: coolc

CoolLexer.cpp: CoolLexer.flex
	flex++ -o CoolLexer.cpp CoolLexer.flex

coolc: CoolLexer.cpp Driver.cpp Token.cpp
	clang++ $(CXXFLAGS) -o $@ Driver.cpp CoolLexer.cpp Token.cpp

clean:
	rm -f coolc
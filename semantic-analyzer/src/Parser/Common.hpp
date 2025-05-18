#pragma once

#include <AST/Program.hpp>
namespace parser {

bool Parse(const char* filename);
Program* GetProgram();

}; // namespace parser
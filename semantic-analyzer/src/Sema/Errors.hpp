#pragma once

#include <string>

class BaseNode;

namespace sema {

std::string quoted(const std::string& str);
void error(std::string_view filename, int line, const std::string& msg);
void error(const std::string& msg);
size_t Line(BaseNode* node);

}; // namespace sema
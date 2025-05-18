cmake_minimum_required(VERSION 3.10)
project(CoolSemanticAnalyzer)

find_package(BISON)
find_package(FLEX)

bison_target(Parser src/Parser/cool.y
            ${CMAKE_CURRENT_BINARY_DIR}/cool-bison-parser.cc
            DEFINES_FILE
            ${CMAKE_CURRENT_BINARY_DIR}/cool-bison-parser.h
            COMPILE_FLAGS
            "-v -y -b cool -p cool_yy")

flex_target(Lexer src/Lexer/cool.flex
            ${CMAKE_CURRENT_BINARY_DIR}/cool-flex-lexer.cc
            )

add_flex_bison_dependency(Lexer Parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(semantic-analyzer
            src/SemanticAnalyzerApp/semantic-analyzer.cpp
            src/Util/Print.cpp
            src/Parser/Parser.cpp
            src/AST/StringTable.cpp
            src/AST/API.cpp
            src/AST/Symbol.cpp
            src/AST/Case.cpp
            src/AST/BaseClasses.cpp
            src/Sema/Errors.cpp
            src/Sema/InheritanceGraphBuilder.cpp
            src/Sema/SemanticChecker.cpp
            ${BISON_Parser_OUTPUTS}
            ${FLEX_Lexer_OUTPUTS} )

target_include_directories(semantic-analyzer PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src)

target_compile_options(semantic-analyzer PRIVATE -Wall -Wno-unused -Wno-deprecated -Wno-write-strings -Wno-free-nonheap-object)

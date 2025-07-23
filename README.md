# Simpl_CompilerV1

Simpl_CompilerV1 is a compiler for the Simpl programming language. It includes a lexer, parser, and evaluator to process Simpl code and execute it. The project is implemented in C++ and uses a custom lexer and parser generator.

## Features
- **Lexer**: Tokenizes Simpl source code into meaningful tokens.
- **Parser**: Parses tokens into an Abstract Syntax Tree (AST).
- **Evaluator**: Executes the AST and evaluates expressions, statements, and functions.
- **Support for Variables and Functions**: Includes global variable declarations, function definitions, and function calls.
- **Control Flow**: Supports `if`, `else`, `while`, and `for` statements.
- **Input/Output**: Includes `print` and `read` functionalities.

## Requirements
- C++17 or later
- CMake 3.10 or later
- fa2lexer (for lexer generation)

## Build Instructions
1. cmake -B build-SimplCompiler -S .
2. cmake --build build-SimplCompiler
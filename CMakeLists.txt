cmake_minimum_required (VERSION 3.10)
project (SimplCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_command (
  OUTPUT SimplLexer.cpp tokens.hpp
  COMMAND fa2lexer --src SimplLexer.cpp --hdr tokens.hpp --tpl ${PROJECT_SOURCE_DIR}/Lexer.fa2l
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/Lexer.fa2l

  DEPENDS ${PROJECT_SOURCE_DIR}/SimplDFA/*.jff
        
)
set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_SOURCE_FILES main.cpp)

set(PROJECT_GENERATED_FILES SimplLexer.cpp
                            tokens.hpp)

add_executable(SimplCompiler ${PROJECT_SOURCE_FILES}
                      ${PROJECT_GENERATED_FILES})

target_include_directories(SimplCompiler PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(SimplCompiler PRIVATE ${PROJECT_SOURCE_DIR})
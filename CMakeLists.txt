cmake_minimum_required(VERSION 3.16)
project(z8asm)

set(This z8asm)

set(CMAKE_C_STANDARD 90)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Headers
  src/lexer/lexer.h
  src/ast/ast.h
)

set(Sources
  src/lexer/lexer.c
  src/ast/ast.c
)

enable_testing()

add_subdirectory(tests)

add_library(${This}_lib STATIC ${Sources} ${Headers})
target_include_directories(${This}_lib PUBLIC src)

add_executable(${This} ${Sources} ${Headers} src/main.c)

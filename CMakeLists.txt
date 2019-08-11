cmake_minimum_required(VERSION 3.0)
project(JITCalc)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set (SRC
  src/compiler.cc
  src/exec-memory.cc
  src/ir.cc
  src/main.cc
  src/parser.cc
  src/x64-writter.cc
)
add_executable(calc ${SRC})

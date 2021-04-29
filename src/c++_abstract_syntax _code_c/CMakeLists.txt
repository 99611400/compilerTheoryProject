cmake_minimum_required(VERSION 3.17)
project(c___abstract_syntax__code_c)

set(CMAKE_CXX_STANDARD 14)

add_executable(abc  productionForm.h productionForm.cpp LL1.cpp LL1.h main.cpp)
add_executable(example example.cpp )
add_executable(test test.cpp )
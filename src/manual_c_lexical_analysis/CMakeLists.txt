cmake_minimum_required(VERSION 3.17)
project(Experiment1)

aux_source_directory("word" WORD)

add_executable(Experiment1 main.cpp ${WORD} Parser.cpp Parser.h)


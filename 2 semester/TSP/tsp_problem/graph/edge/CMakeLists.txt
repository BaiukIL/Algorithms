cmake_minimum_required(VERSION 3.13)
project(Edge)

set(NAME_LIB edge_lib)
set(SOURCE_LIB edge.cpp)

add_library(${NAME_LIB} ${SOURCE_LIB})

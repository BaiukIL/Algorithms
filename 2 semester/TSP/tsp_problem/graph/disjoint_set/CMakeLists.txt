cmake_minimum_required(VERSION 3.13)
project(Disjoint_set)

set(NAME_LIB disjoint_set_lib)
set(SOURCE_LIB disjoint_set.cpp)

add_library(${NAME_LIB} ${SOURCE_LIB})

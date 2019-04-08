cmake_minimum_required(VERSION 3.12)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp igraph.h list_graph.h list_graph.cpp matrix_graph.cpp matrix_graph.h set_graph.cpp set_graph.h arc_graph.cpp arc_graph.h)
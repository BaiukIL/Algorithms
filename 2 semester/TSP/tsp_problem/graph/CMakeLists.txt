cmake_minimum_required(VERSION 3.13)
project(Set_graph)

set(NAME_LIB set_graph_lib)
set(SOURCE_LIB set_graph.cpp graph_funcs.cpp)
set(SUB_DIR disjoint_set)
set(SUB_LIB disjoint_set_lib)

add_subdirectory(${SUB_DIR})

add_library(${NAME_LIB} ${SOURCE_LIB})

target_link_libraries(${NAME_LIB} ${SUB_LIB})

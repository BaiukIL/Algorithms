cmake_minimum_required(VERSION 3.13)
project(TSP_solution)

set(NAME_LIB tsp_solution_lib)
set(SOURCE_LIB exact_tsp_solution.cpp double_tsp_solution.cpp christofides_tsp_solution.cpp)

set(SUB_DIR graph)
set(SUB_LIB set_graph_lib)

add_subdirectory(${SUB_DIR})

add_library(${NAME_LIB} ${SOURCE_LIB})

target_link_libraries(${NAME_LIB} ${SUB_LIB})

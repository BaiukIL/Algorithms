cmake_minimum_required(VERSION 3.13)
project(TSP_statistic)

set(CMAKE_CXX_STANDARD 17)

set(NAME_EXE statistic)
set(SOURCE_EXE statistic.cpp)

set(LIBRARIES tsp_solution_lib)
set(SUBDIRECTORIES tsp_problem)

add_executable(${NAME_EXE} ${SOURCE_EXE})

add_subdirectory(${SUBDIRECTORIES})

target_link_libraries(${NAME_EXE} ${LIBRARIES})

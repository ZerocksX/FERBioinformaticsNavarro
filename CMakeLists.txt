cmake_minimum_required(VERSION 3.14)
project(navarro)

#
set(GA_DIR C:/Users/pavao/Documents/faks/bioinf/lab/GraphAligner)
include_directories(${GA_DIR})

#include_directories(GA C:/Users/pavao/Documents/faks/bioinf/lab/GraphAligner)
#link_directories(GA C:/Users/pavao/Documents/faks/bioinf/lab/GraphAligner)
#link_directories(GA STATIC ${CMAKE_SOURCE_DIR}/../GraphAligner)
#link_directories(${CMAKE_SOURCE_DIR}/../GraphAligner)


set(CMAKE_CXX_STANDARD 14)


set(imports ${GA_DIR}/GfaGraph.cpp ${GA_DIR}/GfaGraph.h ${GA_DIR}/ThreadReadAssertion.cpp ${GA_DIR}/ThreadReadAssertion.h)

add_executable(navarro main.cpp ${imports})
target_include_directories(navarro PRIVATE ${GA_DIR})
#target_link_libraries(navarro ${GA})
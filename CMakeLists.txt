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
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3")

#set(imports ${GA_DIR}/GfaGraph.cpp ${GA_DIR}/GfaGraph.h ${GA_DIR}/ThreadReadAssertion.cpp ${GA_DIR}/ThreadReadAssertion.h)
set(imports ${GA_DIR}/fastqloader.h ${GA_DIR}/fastqloader.cpp ${GA_DIR}/CommonUtils.h ${GA_DIR}/CommonUtils.cpp ${GA_DIR}/ThreadReadAssertion.h ${GA_DIR}/ThreadReadAssertion.cpp GfaGraph.cpp GfaGraph.h Node.cpp Node.h navarro/navarroNode.cpp navarro/navarroNode.h)

add_executable(navarro main.cpp ${imports} )
target_include_directories(navarro PRIVATE ${GA_DIR})
#target_link_libraries(navarro ${GA})
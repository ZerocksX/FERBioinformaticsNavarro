cmake_minimum_required(VERSION 3.14)
project(navarro)

set(CMAKE_CXX_STANDARD 14)


set(imports GraphAligner/fastqloader.h GraphAligner/fastqloader.cpp GraphAligner/CommonUtils.h GraphAligner/CommonUtils.cpp GraphAligner/ThreadReadAssertion.h GraphAligner/ThreadReadAssertion.cpp GfaGraph.cpp GfaGraph.h Node.cpp Node.h navarro/navarroNode.cpp navarro/navarroNode.h main.h)
set(GCC_COVERAGE_COMPILE_FLAGS "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


add_executable(navarro main.cpp ${imports} )
target_include_directories(navarro PRIVATE ${GA_DIR})
#target_link_libraries(navarro ${GA})
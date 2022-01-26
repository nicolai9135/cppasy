cmake_minimum_required(VERSION 3.10)

# set project name
project(cppasy)

add_compile_definitions(EVAL=2)
# add_compile_definitions(SAFE)

find_package(Boost 1.75 COMPONENTS program_options REQUIRED) 

set(Z3_DIR /home/nicolai/RWTH/master/master_thesis/install/z3-master/build)

set(Z3_INCLUDE_DIR /home/nicolai/RWTH/master/master_thesis/install/z3-master/build)
set(Z3_LIBRARY_DIR /home/nicolai/RWTH/master/master_thesis/install/z3-master/build)
find_package(Z3 REQUIRED)

# set output directory for binary
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

target_compile_options(cli PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:
        -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>)
#        -Werror -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>)

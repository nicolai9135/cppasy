cmake_minimum_required(VERSION 3.10)

# set project name
project(cppasy)

add_compile_definitions(EVAL=1)

find_package(Boost 1.75 COMPONENTS program_options REQUIRED) 

find_package(Z3 REQUIRED)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

# set output directory for binary
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)

target_compile_options(cli PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:
        -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>)
#        -Werror -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>)

target_compile_options(gui PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:
        -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>)
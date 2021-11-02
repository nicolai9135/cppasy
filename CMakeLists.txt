cmake_minimum_required(VERSION 3.10)

# set project name
project(cppasy)

find_package(Boost 1.75 COMPONENTS program_options REQUIRED) 

find_package(Z3 REQUIRED)

# set output directory for binary
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
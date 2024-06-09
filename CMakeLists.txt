cmake_minimum_required(VERSION 3.10)
project(aoc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB days src/day*.c)
foreach(day ${days})
    get_filename_component(name ${day} NAME_WE)
    add_executable(${name} ${day} src/util.c)
endforeach()


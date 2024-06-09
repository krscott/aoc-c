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

# Tests

include(CTest)

set(inputs_dir ${PROJECT_SOURCE_DIR}/inputs)

function(do_test target input_file result)
    set(test_name test_${target}_${input_file})
    set(test_input_path ${inputs_dir}/${input_file})
    if (NOT EXISTS ${test_input_path})
        message(SEND_ERROR "Test input does not exist: ${test_input_path}")
    endif()
    add_test(NAME ${test_name} COMMAND ${target} ${test_input_path})
    set_tests_properties(
        ${test_name}
        PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

do_test(day01a 01a-example.txt 142)
do_test(day01a 01.txt 56042)
do_test(day01b 01b-example.txt 281)
do_test(day01b 01.txt 55358)

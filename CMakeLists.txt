cmake_minimum_required(VERSION 3.10)
project(aoc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

function(add_day day part utils)
    get_filename_component(basename ${day} NAME_WE)
    set(target ${basename}${part})
    message(${utils})
    add_executable(${target} ${day} ${utils})
    target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -pedantic)
    if(${part} MATCHES "a")
        target_compile_definitions(${target} PRIVATE PART1=1)
    elseif(${part} MATCHES "b")
        target_compile_definitions(${target} PRIVATE PART1=0)
    else()
        message(SEND_ERROR "Invalid part '${part}', expected 'a' or 'b'")
    endif()
endfunction()

file(GLOB days src/day*.c) 
file(GLOB utils src/util/*.c)
foreach(day ${days})
    add_day(${day} a "${utils}")
    add_day(${day} b "${utils}")
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

do_test(day02a 02-example.txt 8)
do_test(day02a 02.txt 3059)
do_test(day02b 02-example.txt 2286)
do_test(day02b 02.txt 65371)

do_test(day03a 03-example.txt 4361)
do_test(day03a 03.txt 556367)
do_test(day03b 03-example.txt 467835)
do_test(day03b 03.txt 89471771)

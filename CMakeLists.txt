cmake_minimum_required(VERSION 3.10)
project(aoc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

if (DEFINED ENV{LOG_DBG})
    add_compile_definitions(LOG_DBG=$ENV{LOG_DBG})
endif()

add_compile_options(
    -Wall
    -Wextra
    -Werror
    -pedantic
    -Wbad-function-cast
    -Wcast-align
    -Wcast-qual
    -Wformat=2
    -Wnested-externs
    -Wpointer-arith
    -Wredundant-decls
    -Wsequence-point
    -Wshadow
    -Wstrict-prototypes
    -Wswitch
    -Wundef
    -Wunreachable-code
    -Wunused-but-set-parameter
    -Wwrite-strings
    -Wpointer-to-int-cast
    -Wint-to-pointer-cast
    -Wconversion
)

function(define_file_name_macro target)
    get_target_property(sources ${target} SOURCES)
    foreach(source ${sources})
        get_filename_component(source_basename ${source} NAME)
        set_source_files_properties(${source} PROPERTIES COMPILE_DEFINITIONS "FILE_NAME=\"${source_basename}\"")
    endforeach()
endfunction()

function(add_day day part utils)
    get_filename_component(basename ${day} NAME_WE)
    set(target ${basename}${part})
    add_executable(${target} ${day} ${utils})
    if(${part} MATCHES "a")
        target_compile_definitions(${target} PRIVATE PART1=1)
    elseif(${part} MATCHES "b")
        target_compile_definitions(${target} PRIVATE PART1=0)
    else()
        message(SEND_ERROR "Invalid part '${part}', expected 'a' or 'b'")
    endif()
    define_file_name_macro(${target})
endfunction()


file(GLOB days src/day*.c) 
file(GLOB utils src/util/*.c)
list(FILTER utils EXCLUDE REGEX "_test\.c")
foreach(day ${days})
    add_day(${day} a "${utils}")
    add_day(${day} b "${utils}")
endforeach()

# Tests

include(CTest)

file(GLOB util_tests src/util/*_test.c)
foreach(util_test ${util_tests})
    get_filename_component(target ${util_test} NAME_WE)
    add_executable(${target} ${util_test} ${utils})
    define_file_name_macro(${target})
    add_test(NAME ${target} COMMAND ${target})
endforeach()

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

do_test(day04a 04-example.txt 13)
do_test(day04a 04.txt 21568)
do_test(day04b 04-example.txt 30)
do_test(day04b 04.txt 11827296)

do_test(day05a 05-example.txt 35)
do_test(day05a 05.txt 525792406)
do_test(day05b 05-example.txt 46)
do_test(day05b 05.txt 79004094)

do_test(day06a 06-example.txt 288)
do_test(day06a 06.txt 227850)
do_test(day06b 06-example.txt 71503)
do_test(day06b 06.txt 42948149)

do_test(day07a 07-example.txt 6440)
do_test(day07a 07.txt 248422077)
do_test(day07b 07-example.txt 5905)
do_test(day07b 07.txt 249817836)

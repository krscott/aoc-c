#ifndef UTIL_H
#define UTIL_H

#include <assert.h>  // IWYU pragma: export
#include <stdint.h>
#include <stdlib.h>  // IWYU pragma: export

// Defaults for cmake-defined variables
#ifndef PART1
#define PART1 0
#endif
#ifndef FILE_NAME
#define FILE_NAME __FILE__
#endif

#define countof(a) (sizeof(a) / sizeof(*(a)))
#define STR_(s) #s
#define STR(s) STR_(s)

#define NODISCARD __attribute__((warn_unused_result))

#define IGNORE(x) \
    do {          \
        if (x) {  \
        }         \
    } while (0)

#define assert_type(T, v) ((struct { T inner; }){.inner = (v)}.inner)
#define assert_defined(ident, T)                                                                  \
    static_assert(                                                                                \
        sizeof(assert_type(T, ident)) == sizeof(T), "Expected definition: " STR(T) " " STR(ident) \
    )

typedef int64_t i64;
typedef uint32_t u32;

NODISCARD i64 max_i32(i64 a, i64 b);

NODISCARD char const *my_basename(char const *path);

#endif  // UTIL_H

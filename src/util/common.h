#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))

typedef int64_t i64;
inline i64 max_i32(i64 a, i64 b) { return (a > b) ? a : b; }

#endif  // UTIL_H

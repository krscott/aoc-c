#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))

typedef int32_t i32;
inline i32 max_i32(i32 a, i32 b) { return (a > b) ? a : b; }

#endif  // UTIL_H

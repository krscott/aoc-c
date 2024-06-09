#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))

#if PART1
#define part1only
#define part2only __attribute__((unused))
#else
#define part1only __attribute__((unused))
#define part2only
#endif  // PART1

typedef int32_t i32;
inline i32 max_i32(i32 a, i32 b) { return (a > b) ? a : b; }

#endif  // UTIL_H

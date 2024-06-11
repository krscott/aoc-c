#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define countof(a) (sizeof(a) / sizeof(*(a)))

typedef int64_t i64;
i64 max_i32(i64 a, i64 b);

char const *my_basename(char const *path);

#endif  // UTIL_H

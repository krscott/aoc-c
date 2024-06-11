#include "common.h"

i64 max_i32(i64 a, i64 b) { return (a > b) ? a : b; }

char const *my_basename(char const *path) {
    char const *base = path;
    for (; *path; ++path) {
        if (*path == '/' || *path == '\\') {
            base = path + 1;
        }
    }
    return base;
}

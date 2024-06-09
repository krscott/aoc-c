#include <stdlib.h>

#include "util.h"

enum err add_line_calibration(i32 *cal, char const *line, ssize_t length) {
    i32 first = -1;
    i32 last = -1;

    for (size_t i = 0; i < length; ++i) {
        char c = line[i];
        if (c >= '0' && c <= '9') {
            i32 const x = c - '0';
            if (first == -1) first = x;
            last = x;
        }
    }

    if (first == -1) {
        log_err("Could not find calibration: %.*s", (int)length, line);
        return ERR_INPUT;
    }

    *cal += first * 10 + last;

    return OK;
}

int main(int argc, char *argv[]) {
    FILE *file;
    enum err e = cli(&file, argc, argv);
    if (e) return e;

    i32 total = 0;

    char *line = NULL;
    size_t n = 0;
    ssize_t length = 0;

    while ((length = getline(&line, &n, file)) != -1) {
        e = add_line_calibration(&total, line, length);
        if (e) return e;
    }
    free(line);

    printf("%d\n", total);
}

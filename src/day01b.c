#include <assert.h>
#include <string.h>

#include "util.h"

char const *digits[] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
};

i32 get_word_digit(struct str s) {
    assert(s.len > 0);
    for (ssize_t i = 0; i < countof(digits); ++i) {
        for (ssize_t j = 0;; ++j) {
            if (digits[i][j] == 0 || j == s.len) return i;
            if (digits[i][j] != s.ptr[j]) break;
        }
    }
    return -1;
}

enum err add_line_calibration(i32 *cal, struct str line) {
    if (line.len <= 0) return OK;

    i32 first = -1;
    i32 last = -1;

    for (size_t i = 0; i < line.len; ++i) {
        i32 d = -1;
        char c = line.ptr[i];
        if (c >= '0' && c <= '9') {
            d = c - '0';
        } else {
            d = get_word_digit(str_substr(line, i, line.len));
        }
        if (d != -1) {
            if (first == -1) first = d;
            last = d;
        }
    }

    if (first == -1) {
        log_err("Could not find calibration: %.*s", (int)line.len, line.ptr);
        return ERR_INPUT;
    }

    *cal += first * 10 + last;

    return OK;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    i32 total = 0;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e) goto error;
        if (!line.ptr) break;

        e = add_line_calibration(&total, line);
        if (e) goto error;
    }

    printf("%d\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

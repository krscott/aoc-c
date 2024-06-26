#include <assert.h>

#include "util/common.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"

static char const *digits[] = {
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

static i64 get_word_digit(struct str const s) {
    assert(s.len > 0);
    for (size_t i = 0; i < countof(digits); ++i) {
        for (size_t j = 0;; ++j) {
            if (digits[i][j] == 0) return (i64)i;
            if (j == s.len || digits[i][j] != s.ptr[j]) break;
        }
    }
    return -1;
}

static ERRFN add_line_calibration_p2(i64 *const cal_total, struct str const line) {
    assert(cal_total);
    if (line.len <= 0) return OK;

    i64 first = -1;
    i64 last = -1;

    for (size_t i = 0; i < line.len; ++i) {
        i64 d = -1;
        char const c = line.ptr[i];
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

    i64 const line_cal = first * 10 + last;
    log_dbg("%ld", line_cal);

    *cal_total += line_cal;

    return OK;
}

static ERRFN add_line_calibration_p1(i64 *const cal, struct str const line) {
    assert(cal);
    if (line.len <= 0) return OK;

    i64 first = -1;
    i64 last = -1;

    for (size_t i = 0; i < line.len; ++i) {
        char const c = line.ptr[i];
        if (c >= '0' && c <= '9') {
            i64 const x = c - '0';
            if (first == -1) first = x;
            last = x;
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
    i64 total = 0;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e == ERR_NONE) break;
        if (e) goto error;

        if (PART1) {
            e = add_line_calibration_p1(&total, line);
        } else {
            e = add_line_calibration_p2(&total, line);
        }
        if (e) goto error;
    }

    printf("%ld\n", total);

error:
    fileiter_deinit(&f);
    return (int)e;
}

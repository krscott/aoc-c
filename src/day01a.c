#include "util.h"

enum err add_line_calibration(i32 *cal, struct str line) {
    if (line.len <= 0) return OK;

    i32 first = -1;
    i32 last = -1;

    for (size_t i = 0; i < line.len; ++i) {
        char c = line.ptr[i];
        if (c >= '0' && c <= '9') {
            i32 const x = c - '0';
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

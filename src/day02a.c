#include <string.h>

#include "util/common.h"
#include "util/fileiter.h"
#include "util/str.h"

static enum err parse_round(i32 *max_r, i32 *max_g, i32 *max_b, struct str round) {
    assert(max_r && max_g && max_b);
    enum err e = OK;
    for (;;) {
        struct str color;
        char delim = str_split(&color, &round, round, ",");

        i32 count;
        e = str_take_int(&count, &color, color);
        if (e) goto error;

        color = str_trim_whitespace(color);

        if (strncmp("red", color.ptr, color.len) == 0) {
            *max_r = max_i32(*max_r, count);
        } else if (strncmp("green", color.ptr, color.len) == 0) {
            *max_g = max_i32(*max_g, count);
        } else if (strncmp("blue", color.ptr, color.len) == 0) {
            *max_b = max_i32(*max_b, count);
        } else {
            log_err("Invalid color: %.*s", (int)color.len, color.ptr);
            e = ERR_INPUT;
            goto error;
        }

        if (!delim) break;
    }

error:
    return e;
}

static enum err parse_line(i32 *total, struct str line) {
    enum err e = OK;

    // Game
    str_split(NULL, &line, line, " ");

    // ID
    i32 id;
    e = str_take_int(&id, &line, line);
    if (e) goto error;
    str_split(NULL, &line, line, ":");

    i32 max_r = 0;
    i32 max_g = 0;
    i32 max_b = 0;

    for (;;) {
        struct str round;
        char delim = str_split(&round, &line, line, ";");

        e = parse_round(&max_r, &max_g, &max_b, round);
        if (e) goto error;

        if (!delim) break;
    }

    if (max_r <= 12 && max_g <= 13 && max_b <= 14) {
        *total += id;
    }

error:
    return e;
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

        e = parse_line(&total, line);
        if (e) goto error;
    }

    printf("%d\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

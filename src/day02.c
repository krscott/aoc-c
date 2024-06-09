#include <string.h>

#include "util/common.h"
#include "util/error.h"
#include "util/fileiter.h"
#include "util/str.h"

struct linedata {
    i32 id;
    i32 r;
    i32 g;
    i32 b;
};

static enum err parse_round(struct linedata *data, struct str round) {
    assert(data);
    enum err e = OK;
    for (;;) {
        struct str color;
        char delim = str_split(&color, &round, round, ",");

        i32 count;
        e = str_take_int(&count, &color, color);
        if (e) goto error;

        color = str_trim_whitespace(color);

        if (strncmp("red", color.ptr, color.len) == 0) {
            data->r = max_i32(data->r, count);
        } else if (strncmp("green", color.ptr, color.len) == 0) {
            data->g = max_i32(data->g, count);
        } else if (strncmp("blue", color.ptr, color.len) == 0) {
            data->b = max_i32(data->b, count);
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

static enum err linedata_get(struct linedata *data, struct str line) {
    assert(data);

    enum err e = OK;

    // Game
    str_split(NULL, &line, line, " ");

    // ID
    e = str_take_int(&data->id, &line, line);
    if (e) goto error;
    str_split(NULL, &line, line, ":");

    for (;;) {
        struct str round;
        char delim = str_split(&round, &line, line, ";");

        e = parse_round(data, round);
        if (e) goto error;

        if (!delim) break;
    }

error:
    return e;
}

static enum err parse_line(i32 *total, struct str line) {
    struct linedata data = {0};
    enum err e = linedata_get(&data, line);
    if (e) goto error;

#if PART1
    if (data.r <= 12 && data.g <= 13 && data.b <= 14) {
        *total += data.id;
    }
#else
    *total += data.r * data.g * data.b;
#endif

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

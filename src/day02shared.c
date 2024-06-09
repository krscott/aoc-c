#include "day02shared.h"

#include <string.h>

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

enum err linedata_get(struct linedata *data, struct str line) {
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

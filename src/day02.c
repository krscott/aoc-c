#include <string.h>

#include "util/common.h"
#include "util/error.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"

struct linedata {
    i64 id;
    i64 r;
    i64 g;
    i64 b;
};

static ERRFN parse_round(struct linedata *const data, struct str round) {
    assert(data);
    enum err e = OK;
    while (round.len > 0) {
        struct str color = str_split(&round, round, ",");

        i64 count;
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
    }

error:
    return e;
}

static ERRFN linedata_get(struct linedata *const data, struct str line) {
    assert(data);
    enum err e = OK;

    // Game
    str_split(&line, line, " ");

    // ID
    e = str_take_int(&data->id, &line, line);
    if (e) goto error;
    str_split(&line, line, ":");

    while (line.len > 0) {
        struct str const round = str_split(&line, line, ";");
        e = parse_round(data, round);
        if (e) goto error;
    }

error:
    return e;
}

static ERRFN parse_line(i64 *const total, struct str const line) {
    assert(total);
    struct linedata data = {0};
    enum err e = linedata_get(&data, line);
    if (e) goto error;

    if (PART1) {
        if (data.r <= 12 && data.g <= 13 && data.b <= 14) {
            *total += data.id;
        }
    } else {
        *total += data.r * data.g * data.b;
    }

error:
    return e;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    {
        i64 total = 0;

        for (;;) {
            struct str line;
            e = fileiter_line(&line, &f);
            if (e == ERR_NONE) break;
            if (e) goto error;

            e = parse_line(&total, line);
            if (e) goto error;
        }

        printf("%ld\n", total);
    }

error:
    fileiter_deinit(&f);
    return (int)e;
}

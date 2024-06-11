#include "util/error.h"
#include "util/fileiter.h"
#include "util/intvec.h"
#include "util/str.h"
#include "util/vec.h"

static ssize_t const expected_maps = 7;

struct range {
    i64 dest;
    i64 src;
    i64 len;
};

vec_define_struct(rangevec, struct range);

static enum err parse_seeds(struct intvec *seeds, struct fileiter *f) {
    enum err e = OK;
    struct str line;
    e = fileiter_line(&line, f);
    if (e) return e;
    if (!line.ptr) return err_trace(ERR_INPUT);

    str_split(&line, line, ":");

    i64 n;
    for (;;) {
        switch (e = str_take_int(&n, &line, line)) {
            case OK:
                vec_push(seeds, n);
                break;
            case ERR_NONE:
                return OK;
            default:
                return err_trace(e);
        }
    }
}

static enum err parse_range(struct range *range, struct str line) {
    return str_take_int(&range->dest, &line, line)    //
           || str_take_int(&range->src, &line, line)  //
           || str_take_int(&range->len, &line, line);
}

static enum err parse_ranges(struct rangevec *ranges, struct fileiter *f) {
    enum err e = OK;
    struct str line = {0};

    // Skip blank lines and header
    while (line.len == 0) {
        e = fileiter_line(&line, f);
        if (e) return e;
    }

    struct range range;
    for (;;) {
        e = fileiter_line(&line, f);
        if (e) return e;
        switch (e = parse_range(&range, line)) {
            case OK:
                vec_push(ranges, range);
                break;
            case ERR_NONE:
                return OK;
            default:
                return e;
        }
    }
}

static void transform_seeds(struct intvec *seeds, struct rangevec const *ranges) {
    for (ssize_t i = 0; i < seeds->len; ++i) {
        i64 seed = seeds->buf[i];
        for (ssize_t j = 0; j < ranges->len; ++j) {
            struct range const range = ranges->buf[j];
            if (range.src <= seed && seed < range.src + range.len) {
                seeds->buf[i] = seed - range.src + range.dest;
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    struct intvec seeds = {0};
    struct rangevec ranges = {0};

    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    e = parse_seeds(&seeds, &f);
    if (e) goto error;

    intvec_log_dbg(seeds);

    ssize_t map_i = 0;
    for (; e != ERR_NONE; ++map_i) {
        vec_clear(&ranges);
        e = parse_ranges(&ranges, &f);
        if (e > ERR_NONE) goto error;
        transform_seeds(&seeds, &ranges);

        intvec_log_dbg(seeds);
    }

    // Make sure we've used all maps
    if (map_i != expected_maps) {
        log_err("Expected %zd maps, got %zd", expected_maps, map_i);
        return ERR_INPUT;
    }

    i64 ans = 0;
    e = intvec_min(&ans, seeds);
    printf("%ld\n", ans);

error:
    vec_deinit(&ranges);
    vec_deinit(&seeds);
    fileiter_deinit(&f);
    return e;
}

#include "util/error.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"
#include "util/vec.h"

#if LOG_DBG
#include "util/intvec.h"
#endif

static ssize_t const expected_maps = 7;

struct seed {
    i64 start;
    i64 len;
};
vec_define_struct(seedvec, struct seed);

struct range {
    i64 dest;
    i64 src;
    i64 len;
};
vec_define_struct(rangevec, struct range);

static enum err parse_seeds(struct seedvec *seeds, struct fileiter *f) {
    enum err e = OK;
    struct str line;
    e = fileiter_line(&line, f);
    if (e) return e;
    if (!line.ptr) return err_trace(ERR_INPUT);

    str_split(&line, line, ":");

    struct seed seed = {0};
    for (;;) {
        switch (e = str_take_int(&seed.start, &line, line)) {
            case OK:
                break;
            case ERR_NONE:
                return OK;
            default:
                return err_trace(e);
        }
        if (PART1) {
            seed.len = 1;
        } else {
            switch (e = str_take_int(&seed.len, &line, line)) {
                case OK:
                    break;
                case ERR_NONE:
                    return err_trace(ERR_INPUT);
                default:
                    return err_trace(e);
            }
        }
        vec_push(seeds, seed);
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

static void transform_seeds(struct seedvec *seeds, struct rangevec_slice const ranges) {
    for (ssize_t i = 0; i < seeds->len; ++i) {
        struct seed seed = seeds->ptr[i];
        for (ssize_t j = 0; j < ranges.len; ++j) {
            struct range const range = ranges.ptr[j];
            i64 range_end = range.src + range.len;
            i64 seed_end = seed.start + seed.len;

            // Split seed range
            if (range.src > seed.start && range.src < seed_end) {
                struct seed old_seed = {
                    .start = seed.start,
                    .len = range.src - seed.start,
                };
                struct seed new_seed = {
                    .start = range.src,
                    .len = seed.len - old_seed.len,
                };
                log_dbg(
                    "Split: {%ld,%ld} x {%ld,%ld} -> {%ld,%ld}, {%ld,%ld}",
                    seed.start,
                    seed.len,
                    range.src,
                    range.len,
                    old_seed.start,
                    old_seed.len,
                    new_seed.start,
                    new_seed.len
                );
                vec_push(seeds, new_seed);
                seeds->ptr[i] = seed = old_seed;
            } else if (range_end > seed.start && range_end < seed_end) {
                struct seed old_seed = {
                    .start = seed.start,
                    .len = range_end - seed.start,
                };
                struct seed new_seed = {
                    .start = range_end,
                    .len = seed.len - old_seed.len,
                };
                log_dbg(
                    "Split: {%ld,%ld} x {%ld,%ld} -> {%ld,%ld}, {%ld,%ld}",
                    seed.start,
                    seed.len,
                    range.src,
                    range.len,
                    old_seed.start,
                    old_seed.len,
                    new_seed.start,
                    new_seed.len
                );
                vec_push(seeds, new_seed);
                seeds->ptr[i] = seed = old_seed;
            }

            // Transform seed start
            if (range.src <= seed.start && seed.start < range_end) {
                struct seed moved_seed = {
                    .start = seed.start - range.src + range.dest,
                    .len = seed.len,
                };
                log_dbg(
                    "Move: {%ld,%ld} x {%ld->%ld,%ld} -> {%ld,%ld}",
                    seed.start,
                    seed.len,
                    range.src,
                    range.dest,
                    range.len,
                    moved_seed.start,
                    moved_seed.len
                );
                seeds->ptr[i] = moved_seed;
                break;
            }
        }
    }
}

static inline void seedvec_log_debug(struct seedvec seeds) {
#if LOG_DBG
    struct intvec seed_flat = {0};
    for (ssize_t i = 0; i < seeds.len; ++i) {
        vec_push(&seed_flat, seeds.ptr[i].start);
        vec_push(&seed_flat, seeds.ptr[i].len);
    }
    intvec_log_dbg(seed_flat);
    vec_deinit(&seed_flat);
#else
    (void)seeds;
#endif
}

enum err get_min_seed(i64 *min, struct seedvec *seeds) {
    ssize_t len = seeds->len;
    struct seed *buf = seeds->ptr;
    if (len == 0) return err_trace(ERR_INPUT);
    i64 x = buf[0].start;
    for (ssize_t i = 1; i < len; ++i) {
        i64 val = buf[i].start;
        if (x > val) {
            x = val;
        }
    }
    *min = x;
    return OK;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    struct seedvec seeds = {0};
    struct rangevec ranges = {0};

    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    e = parse_seeds(&seeds, &f);
    if (e) goto error;

    seedvec_log_debug(seeds);

    ssize_t map_i = 0;
    for (; e != ERR_NONE; ++map_i) {
        vec_clear(&ranges);
        e = parse_ranges(&ranges, &f);
        if (e > ERR_NONE) goto error;
        transform_seeds(&seeds, vec_slice(rangevec, &ranges));

        seedvec_log_debug(seeds);
    }

    // Make sure we've used all maps
    if (map_i != expected_maps) {
        log_err("Expected %zd maps, got %zd", expected_maps, map_i);
        return ERR_INPUT;
    }

    i64 min;
    e = get_min_seed(&min, &seeds);
    if (e) goto error;

    printf("%ld\n", min);

error:
    vec_deinit(&ranges);
    vec_deinit(&seeds);
    fileiter_deinit(&f);
    return e;
}

#include "util/error.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"
#include "util/vec.h"

struct race {
    i64 time;
    i64 distance;
};
vec_define_struct(racevec, struct race);

static ERRFN parse_races(struct racevec *const races, struct str times, struct str distances) {
    assert(races);
    *races = (struct racevec){0};

    str_split(&times, times, ":");
    str_split(&distances, distances, ":");

    enum err e = OK;
    while (e == OK) {
        struct race race;
        e = str_take_int(&race.time, &times, times)                 //
            || str_take_int(&race.distance, &distances, distances)  //
            || vec_push(racevec, races, race);
        if (e > ERR_NONE) return e;
    }

    return OK;
}

static i64 race_get_wins(struct race const race) {
    i64 test_time = race.time / 2;
    i64 test_distance;
    i64 wins = 0;
    for (; test_time > 0;) {
        test_distance = (race.time - test_time) * test_time;
        if (test_distance <= race.distance) break;
        ++wins;
        --test_time;
    }

    wins *= 2;
    if (wins > 0 && race.time % 2 == 0) wins -= 1;

    log_dbg("Wins: %ld", wins);

    return wins;
}

int main(int argc, char *argv[]) {
    struct linevec lines;
    enum err e = cli_file_lines(&lines, argc, argv);
    if (e) goto error;

    if (lines.len != 2) {
        log_err("Expected 2 lines, got %zd", lines.len);
        e = ERR_INPUT;
        goto error;
    }

    if (!PART1) {
        e = strbuf_replace_all(&lines.ptr[0], " ", "")      //
            || strbuf_replace_all(&lines.ptr[1], " ", "");  //
        if (e) goto error;
        str_println(strbuf_to_str(lines.ptr[0]));
        str_println(strbuf_to_str(lines.ptr[1]));
    }

    struct racevec races;
    e = parse_races(&races, strbuf_to_str(lines.ptr[0]), strbuf_to_str(lines.ptr[1]));
    if (e) goto error;

    {
        i64 wins = 1;

        for (size_t i = 0; i < races.len; ++i) {
            wins *= race_get_wins(races.ptr[i]);
        }

        printf("%ld\n", wins);
    }

error:
    vec_deinit(racevec, &races);
    linevec_deinit(&lines);
    return (int)e;
}

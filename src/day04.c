#include "util/error.h"
#include "util/fileiter.h"
#include "util/intvec.h"
#include "util/log.h"
#include "util/str.h"
#include "util/vec.h"

static ERRFN get_card_wins(i64 *wins, struct str line) {
    assert(wins);
    *wins = 0;

    enum err e = OK;
    struct intvec winners = {0};
    str_split(&line, line, ":");

    struct str hand_str;
    struct str winners_str = str_split(&hand_str, line, "|");

    for (;;) {
        i64 winner_num;
        e = str_take_int(&winner_num, &winners_str, winners_str);
        if (e == ERR_NONE) {
            e = OK;
            break;
        }
        if (e) goto error;
        e = vec_push(&winners, winner_num);
        if (e) goto error;
    }

    for (;;) {
        i64 hand_num;
        e = str_take_int(&hand_num, &hand_str, hand_str);
        if (e == ERR_NONE) {
            e = OK;
            break;
        }
        if (e) goto error;

        for (ssize_t i = 0; i < winners.len; ++i) {
            if (winners.ptr[i] == hand_num) {
                *wins += 1;
                break;
            }
        }
    }

error:
    vec_deinit(&winners);
    return e;
}

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    struct intvec card_wins = {0};
    struct intvec card_count = {0};

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e == ERR_NONE) break;
        if (e) goto error;

        i64 wins;
        e = get_card_wins(&wins, line);
        if (e) goto error;

        e = vec_push(&card_wins, wins);
        if (e) goto error;

        if (!PART1) {
            e = vec_push(&card_count, 1);
            if (e) goto error;
        }
    }

    i64 total = 0;
    for (ssize_t i = 0; i < card_wins.len; ++i) {
        i64 wins = card_wins.ptr[i];
        if (PART1) {
            if (wins > 0) total += 1 << (wins - 1);
        } else {
            i64 count = card_count.ptr[i];
            for (ssize_t j = 1; j <= wins; ++j) {
                if (i + j >= card_count.len) {
                    log_err("Card %zd counted past last card", i);
                    e = ERR_INPUT;
                    goto error;
                }
                card_count.ptr[i + j] += count;
            }
            total += count;
        }
    }

    printf("%ld\n", total);

error:
    vec_deinit(&card_wins);
    vec_deinit(&card_count);
    fileiter_deinit(&f);
    return e;
}

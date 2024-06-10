#include "util/error.h"
#include "util/fileiter.h"
#include "util/intvec.h"
#include "util/str.h"
#include "util/vec.h"

enum err get_card_wins(i32 *wins, struct str line) {
    assert(wins);
    *wins = 0;

    enum err e = OK;
    struct intvec winners = {0};
    str_split(NULL, &line, line, ":");

    struct str winners_str;
    struct str hand_str;
    str_split(&winners_str, &hand_str, line, "|");

    for (;;) {
        i32 winner_num;
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
        i32 hand_num;
        e = str_take_int(&hand_num, &hand_str, hand_str);
        if (e == ERR_NONE) {
            e = OK;
            break;
        }
        if (e) goto error;

        for (ssize_t i = 0; i < winners.len; ++i) {
            if (winners.buf[i] == hand_num) {
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

    i32 total = 0;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e) goto error;
        if (!line.ptr) break;

        i32 wins;
        e = get_card_wins(&wins, line);
        if (e) goto error;
        if (wins > 0) total += 1 << (wins - 1);
    }

    printf("%d\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

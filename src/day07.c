#include "util/error.h"
#include "util/fileiter.h"
#include "util/log.h"
#include "util/str.h"
#include "util/vec.h"

#define HAND_CARD_COUNT 5
#define CARD_KIND_COUNT 14

enum strength {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
};

struct hand {
    i64 bid;
    enum strength strength;
    char cards[HAND_CARD_COUNT];
};
vec_define_struct(handvec, struct hand);

static int card_kind(char const card) {
    // Offset kinds by 1 to reserve spot for joker at the front

    if ('2' <= card && card <= '9') {
        return card - '2' + 1;
    }
    switch (card) {
        case 'T':
            return 10 - 1;
        case 'J':
            if (PART1) {
                return 11 - 1;
            } else {
                return 0;
            }
        case 'Q':
            return 12 - 1;
        case 'K':
            return 13 - 1;
        case 'A':
            return 14 - 1;
        default:
            return -1;
    }
}

static int hand_cmp(void const* const left, void const* const right) {
    struct hand const* const a = (struct hand const*)left;
    struct hand const* const b = (struct hand const*)right;

    if (a->strength != b->strength) {
        return (int)a->strength - (int)b->strength;
    }

    for (size_t i = 0; i < HAND_CARD_COUNT; ++i) {
        int a_score = card_kind(a->cards[i]);
        assert(a_score >= 0);
        int b_score = card_kind(b->cards[i]);
        assert(b_score >= 0);
        if (a_score != b_score) {
            return a_score - b_score;
        }
    }

    return 0;
}

static enum strength
get_strength_with_joker(int const card_counts[HAND_CARD_COUNT], int const joker) {
    enum strength strength = HIGH_CARD;

    for (size_t i = PART1 ? 0 : 1; i < CARD_KIND_COUNT; ++i) {
        int joker_count = (!PART1 && joker == (int)i) ? card_counts[0] : 0;
        switch (card_counts[i] + joker_count) {
            case 0:
            case 1:
                break;
            case 2:
                switch (strength) {
                    case HIGH_CARD:
                        strength = ONE_PAIR;
                        break;
                    case ONE_PAIR:
                        strength = TWO_PAIR;
                        break;
                    case THREE_OF_A_KIND:
                        strength = FULL_HOUSE;
                        break;
                    default:
                        assert(0);
                }
                break;
            case 3:
                switch (strength) {
                    case HIGH_CARD:
                        strength = THREE_OF_A_KIND;
                        break;
                    case ONE_PAIR:
                        strength = FULL_HOUSE;
                        break;
                    default:
                        assert(0);
                }
                break;
            case 4:
                switch (strength) {
                    case HIGH_CARD:
                        strength = FOUR_OF_A_KIND;
                        break;
                    default:
                        assert(0);
                }
                break;
            case 5:
                switch (strength) {
                    case HIGH_CARD:
                        strength = FIVE_OF_A_KIND;
                        break;
                    default:
                        assert(0);
                }
                break;
            default:
                assert(0);
        }
    }

    return strength;
}

static enum strength get_strength(int const card_counts[HAND_CARD_COUNT]) {
    if (PART1 || card_counts[0] == 0) {
        enum strength const strength = get_strength_with_joker(card_counts, -1);
        log_dbg("Strength: %d", strength);
        return strength;
    } else {
        // TODO: Optimize based on max possible from non-joker score and joker count

        enum strength max_strength = HIGH_CARD;
        for (int joker = 1; joker < CARD_KIND_COUNT; ++joker) {
            enum strength strength = get_strength_with_joker(card_counts, joker);
            if (max_strength < strength) {
                max_strength = strength;
            }
        }
        log_dbg("Strength: %d", max_strength);
        return max_strength;
    }
}

static ERRFN parse_hand(struct hand* const hand, struct str const line) {
    assert(hand);

    // Split line into cards and bid
    struct str bid_str;
    struct str const cards_str = str_split(&bid_str, line, " ");

    // Parse cards

    if (cards_str.len != HAND_CARD_COUNT) {
        log_err("Expected %d cards, got: %.*s", HAND_CARD_COUNT, (int)line.len, line.ptr);
        return ERR_INPUT;
    }

    int card_counts[CARD_KIND_COUNT] = {0};
    for (size_t i = 0; i < HAND_CARD_COUNT; ++i) {
        char const card = cards_str.ptr[i];

        int kind = card_kind(card);

        if (kind < 0) {
            log_err("Unexpected card '%c' in hand: %.*s", card, (int)line.len, line.ptr);
            return ERR_INPUT;
        }

        assert(0 <= kind && kind < CARD_KIND_COUNT);
        ++card_counts[(size_t)kind];
        hand->cards[i] = card;
    }

    // Determine strength

    hand->strength = get_strength(card_counts);

    // Parse bid

    enum err const e = str_take_int(&hand->bid, NULL, bid_str);
    if (e == ERR_NONE) {
        log_err("Could not parse bid: %.*s", (int)line.len, line.ptr);
        return ERR_INPUT;
    } else {
        return e;
    }
}

int main(int argc, char* argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    struct handvec hands = {0};
    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e == ERR_NONE) break;
        if (e) goto error;

        struct hand hand;
        e = parse_hand(&hand, line);
        if (e) goto error;

        e = vec_push(handvec, &hands, hand);
        if (e) goto error;
    }

    vec_sort(handvec, &hands, hand_cmp);

    i64 total = 0;
    for (size_t i = 0; i < hands.len; ++i) {
        total += hands.ptr[i].bid * ((i64)i + 1);
    }
    printf("%ld\n", total);

error:
    vec_deinit(handvec, &hands);
    fileiter_deinit(&f);
    return (int)e;
}

#include <ctype.h>
#include <stdbool.h>

#include "util/fileiter.h"
#include "util/intvec.h"
#include "util/vec.h"

static bool is_symbol(char const c) {
    if (PART1) {
        return c != '.' && !isdigit(c);
    } else {
        return c == '*';
    }
}

static i64 get_number(struct strbuf *const line, size_t const col) {
    assert(line);
    if (col >= line->len || !isdigit(line->ptr[col])) return 0;

    size_t left = col;
    while (left > 0 && isdigit(line->ptr[left - 1])) --left;

    char *start = &line->ptr[left];
    char *end;
    i64 n = strtol(start, &end, 10);
    /* printf("get_number: %zd %d\n", col, n); */
    if (PART1) {
        // Blank out number to prevent double-counting
        /* printf("%s -> ", line->ptr); */
        for (; start < end; ++start) {
            *start = 'x';
        }
    }
    /* printf("%s\n", line->ptr); */
    return n;
}

static ERRFN push_number(struct intvec *const nums, struct strbuf *const line, size_t const col) {
    assert(nums);
    assert(line);
    i64 const n = get_number(line, col);
    if (n) return vec_push(intvec, nums, n);
    return OK;
}

static ERRFN
push_row_adjacent_numbers(struct intvec *const nums, struct strbuf *const line, size_t const col) {
    assert(nums);
    assert(line);
    enum err e = OK;
    // If a number exists in the center, then there can be only one number
    i64 n = get_number(line, col);
    if (n) return vec_push(intvec, nums, n);

    if (col > 0) {
        n = get_number(line, col - 1);
        if (n) e = vec_push(intvec, nums, n);
        if (e) return e;
    }

    n = get_number(line, col + 1);
    if (n) e = vec_push(intvec, nums, n);
    return e;
}

static ERRFN get_adjacent_numbers(
    struct intvec *const nums,
    struct linevec *const lines,
    size_t const row,
    size_t const col
) {
    assert(nums);
    assert(lines);
    enum err e = OK;
    // Top
    if (row > 0) {
        e = push_row_adjacent_numbers(nums, &lines->ptr[row - 1], col);
        if (e) return e;
    }
    // Left
    if (col > 0) {
        e = push_number(nums, &lines->ptr[row], col - 1);
        if (e) return e;
    }
    // Right
    e = push_number(nums, &lines->ptr[row], col + 1);
    if (e) return e;
    // Bottom
    if (lines->len > 0 && row < lines->len - 1) {
        e = push_row_adjacent_numbers(nums, &lines->ptr[row + 1], col);
        if (e) return e;
    }

    return OK;
}

int main(int argc, char *argv[]) {
    struct linevec lines = {0};
    struct intvec nums = {0};

    enum err e = cli_file_lines(&lines, argc, argv);
    if (e) goto error;

    {
        i64 total = 0;

        for (size_t row = 0; row < lines.len; ++row) {
            struct strbuf const line = lines.ptr[row];
            for (size_t col = 0; col < line.len; ++col) {
                char const value = line.ptr[col];
                if (is_symbol(value)) {
                    vec_clear(intvec, &nums);
                    e = get_adjacent_numbers(&nums, &lines, row, col);
                    if (e) goto error;
                    if (PART1) {
                        total += intvec_sum(&nums);
                    } else {
                        if (nums.len == 2) {
                            total += intvec_product(&nums);
                        }
                    }
                }
            }
        }

        printf("%ld\n", total);
    }

error:
    vec_deinit(intvec, &nums);
    linevec_deinit(&lines);
    return (int)e;
}

#include <ctype.h>
#include <stdbool.h>

#include "util/fileiter.h"
#include "util/intvec.h"
#include "util/vec.h"

bool is_symbol(char const c) {
    if (PART1) {
        return c != '.' && !isdigit(c);
    } else {
        return c == '*';
    }
}

i64 get_number(struct strbuf *line, ssize_t const col) {
    if (col < 0 || col >= line->len || !isdigit(line->ptr[col])) return 0;

    ssize_t left = col;
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

enum err push_number(struct intvec *nums, struct strbuf *line, ssize_t col) {
    i64 n = get_number(line, col);
    if (n) return vec_push(nums, n);
    return OK;
}

enum err push_row_adjacent_numbers(struct intvec *nums, struct strbuf *line, ssize_t col) {
    enum err e = OK;
    // If a number exists in the center, then there can be only one number
    i64 n = get_number(line, col);
    if (n) return vec_push(nums, n);

    n = get_number(line, col - 1);
    if (n) e = vec_push(nums, n);
    if (e) return e;

    n = get_number(line, col + 1);
    if (n) e = vec_push(nums, n);
    return e;
}

enum err get_adjacent_numbers(
    struct intvec *nums,
    struct linevec *lines,
    ssize_t const row,
    ssize_t const col
) {
    enum err e = OK;
    // Top
    if (row > 0) {
        e = push_row_adjacent_numbers(nums, &lines->ptr[row - 1], col);
        if (e) return e;
    }
    // Left
    e = push_number(nums, &lines->ptr[row], col - 1);
    if (e) return e;
    // Right
    e = push_number(nums, &lines->ptr[row], col + 1);
    if (e) return e;
    // Bottom
    if (row < lines->len - 1) {
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

    i64 total = 0;

    for (ssize_t row = 0; row < lines.len; ++row) {
        struct strbuf line = lines.ptr[row];
        for (ssize_t col = 0; col < line.len; ++col) {
            char const value = line.ptr[col];
            if (is_symbol(value)) {
                vec_clear(&nums);
                e = get_adjacent_numbers(&nums, &lines, row, col);
                if (e) goto error;
                if (PART1) {
                    total += intvec_sum(nums);
                } else {
                    if (nums.len == 2) {
                        total += intvec_product(nums);
                    }
                }
            }
        }
    }

    printf("%ld\n", total);

error:
    vec_deinit(&nums);
    linevec_deinit(&lines);
    return e;
}

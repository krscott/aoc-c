#include <ctype.h>
#include <stdbool.h>

#include "util/fileiter.h"

bool is_symbol(char const c) { return c != '.' && !isdigit(c); }

i32 get_number(struct cstrbuf *line, ssize_t const col) {
    if (col < 0 || col >= line->len || !isdigit(line->ptr[col])) return 0;

    ssize_t left = col;
    while (left > 0 && isdigit(line->ptr[left - 1])) --left;

    char *start = &line->ptr[left];
    char *end;
    i32 n = strtol(start, &end, 10);
    /* printf("get_number: %zd %d\n", col, n); */
    // Blank out number to prevent double-counting
    /* printf("%s -> ", line->ptr); */
    for (; start < end; ++start) {
        *start = 'x';
    }
    /* printf("%s\n", line->ptr); */
    return n;
}

i32 get_row_adjacent_numbers(struct cstrbuf *line, ssize_t col) {
    // If a number exists in the center, then there can be only one number
    i32 n = get_number(line, col);
    if (!n) {
        n = get_number(line, col - 1) + get_number(line, col + 1);
    }
    return n;
}

i32 get_adjacent_numbers(struct linevec *lines, ssize_t const row, ssize_t const col) {
    i32 total = 0;
    // Top
    if (row > 0) {
        total += get_row_adjacent_numbers(&lines->buf[row - 1], col);
    }
    // Left
    total += get_number(&lines->buf[row], col - 1);
    // Right
    total += get_number(&lines->buf[row], col + 1);
    // Bottom
    if (row < lines->len - 1) {
        total += get_row_adjacent_numbers(&lines->buf[row + 1], col);
    }
    return total;
}

int main(int argc, char *argv[]) {
    struct linevec lines;
    enum err e = cli_file_lines(&lines, argc, argv);
    if (e) goto error;

    i32 total = 0;

    for (ssize_t row = 0; row < lines.len; ++row) {
        struct cstrbuf const line = lines.buf[row];
        for (ssize_t col = 0; col < line.len; ++col) {
            char const value = line.ptr[col];
            if (is_symbol(value)) {
                total += get_adjacent_numbers(&lines, row, col);
            }
        }
    }

    /* printf("\n"); */
    /* for (ssize_t row = 0; row < lines.len; ++row) { */
    /*     printf("%.*s\n", (int)lines.buf[row].len, lines.buf[row].ptr); */
    /* } */

    printf("%d\n", total);

error:
    linevec_deinit(&lines);
    return e;
}

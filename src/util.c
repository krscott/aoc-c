#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum err cli_file(FILE **file, int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s INPUT_FILE\n", argv[0]);
        return ERR_CLI;
    }

    *file = fopen(argv[1], "r");
    if (*file == NULL) {
        fprintf(stderr, "Could not read file: %s\n", argv[1]);
        return ERR_FS;
    }

    return OK;
}

enum err file_iter_init_cli(struct file_iter *iter, int argc, char *argv[]) {
    assert(iter);
    *iter = (struct file_iter){0};
    return cli_file(&iter->file, argc, argv);
}

struct str file_iter_line(struct file_iter *iter) {
    ssize_t line_length = getline(&iter->line, &iter->size, iter->file);
    if (line_length < 0) {
        return (struct str){0};
    }
    return (struct str){
        .ptr = iter->line,
        .len = line_length,
    };
}

void file_iter_deinit(struct file_iter *iter) {
    if (iter->file) fclose(iter->file);
    if (iter->line) free(iter->line);
}

char const *error_string(enum err e) {
    switch (e) {
        case OK:
            return "OK";
        case ERR_CLI:
            return "Command Line Argument Error";
        case ERR_FS:
            return "Filesystem Error";
        case ERR_INPUT:
            return "Invalid Input";
    }
}

/// Get substring from start to end (exclusive)
struct str str_substr(struct str s, ssize_t start, ssize_t end) {
    assert(end > start);
    assert(start >= 0 && start < s.len);
    assert(end > 0 && end <= s.len);
    return (struct str){
        .ptr = s.ptr + start,
        .len = end - start,
    };
}

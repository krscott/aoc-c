#include "util.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

enum err fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]) {
    assert(iter);
    *iter = (struct fileiter){0};
    return cli_file(&iter->file, argc, argv);
}

enum err fileiter_line(struct str *s, struct fileiter *iter) {
    ssize_t line_length = getline(&iter->line, &iter->size, iter->file);
    if (line_length < 0) {
        *s = (struct str){0};
    } else {
        *s = (struct str){
            .ptr = iter->line,
            .len = line_length,
        };
    }
    if (errno) {
        log_err("getline failed: %s", strerror(errno));
        return ERR_ERRNO;
    }
    return OK;
}

void fileiter_deinit(struct fileiter *iter) {
    if (iter->file) fclose(iter->file);
    if (iter->line) free(iter->line);
}

char const *err_string(enum err e) {
    switch (e) {
        case OK:
            return "OK";
        case ERR_CLI:
            return "Command Line Argument Error";
        case ERR_FS:
            return "Filesystem Error";
        case ERR_INPUT:
            return "Invalid Input";
        case ERR_ERRNO:
            return strerror(errno);
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

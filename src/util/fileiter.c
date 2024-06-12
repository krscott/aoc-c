#include "fileiter.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "log.h"
#include "str.h"
#include "vec.h"

void linevec_deinit(struct linevec *lines) {
    for (ssize_t i = 0; i < lines->len; ++i) {
        strbuf_deinit(&lines->ptr[i]);
    }
    vec_deinit(lines);
}

NODISCARD char linevec_get(struct linevec const lines, ssize_t const row, ssize_t const col) {
    assert(row >= 0);
    assert(col >= 0);
    if (row < 0 || row >= lines.len) return '\0';
    struct strbuf const line = lines.ptr[row];
    if (col < 0 || col >= line.len) return '\0';
    return line.ptr[col];
}

ERRFN cli_file(FILE **file, int argc, char *argv[]) {
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

ERRFN cli_file_lines(struct linevec *lines, int argc, char *argv[]) {
    *lines = (struct linevec){0};

    struct fileiter iter;
    enum err e = fileiter_init_cli(&iter, argc, argv);
    if (e) goto error;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &iter);
        if (e == ERR_NONE) {
            e = OK;
            break;
        }
        if (e) goto error;

        struct strbuf linebuf;
        e = strbuf_init_copy_str(&linebuf, str_trim_whitespace(line));
        if (e) goto error;

        e = vec_push(lines, linebuf);
        if (e) goto error;
    }

error:
    fileiter_deinit(&iter);
    return e;
}

ERRFN fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]) {
    assert(iter);
    *iter = (struct fileiter){0};
    return cli_file(&iter->file, argc, argv);
}

ERRFN fileiter_line(struct str *s, struct fileiter *iter) {
    ssize_t line_length = getline(&iter->buffer, &iter->size, iter->file);
    if (errno) {
        log_err("getline failed: %s", strerror(errno));
        *s = (struct str){0};
        return ERR_ERRNO;
    }
    if (line_length < 0) {
        *s = (struct str){0};
        return ERR_NONE;
    }
    *s = str_trim_whitespace((struct str){
        .ptr = iter->buffer,
        .len = line_length,
    });
    str_log_dbg(*s);
    return OK;
}

void fileiter_deinit(struct fileiter *iter) {
    if (iter->file) fclose(iter->file);
    if (iter->buffer) free(iter->buffer);
}

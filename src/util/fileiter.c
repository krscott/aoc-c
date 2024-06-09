#include "fileiter.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "vec.h"

void linevec_deinit(struct linevec *lines) {
    for (ssize_t i = 0; i < lines->len; ++i) {
        cstrbuf_deinit(&lines->buf[i]);
    }
    vec_deinit(lines);
}

char linevec_get(struct linevec const lines, ssize_t const row, ssize_t const col) {
    assert(row >= 0);
    assert(col >= 0);
    if (row < 0 || row >= lines.len) return '\0';
    struct cstrbuf const line = lines.buf[row];
    if (col < 0 || col >= line.len) return '\0';
    return line.ptr[col];
}

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

enum err cli_file_lines(struct linevec *lines, int argc, char *argv[]) {
    *lines = (struct linevec){0};

    struct fileiter iter;
    enum err e = fileiter_init_cli(&iter, argc, argv);
    if (e) goto error;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &iter);
        if (e) goto error;
        if (!line.ptr) break;

        struct cstrbuf linebuf;
        cstrbuf_init_copy_str(&linebuf, str_trim_whitespace(line));

        e = vec_push(lines, linebuf);
        if (e) goto error;
    }

error:
    fileiter_deinit(&iter);
    return e;
}

enum err fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]) {
    assert(iter);
    *iter = (struct fileiter){0};
    return cli_file(&iter->file, argc, argv);
}

enum err fileiter_line(struct str *s, struct fileiter *iter) {
    ssize_t line_length = getline(&iter->buffer, &iter->size, iter->file);
    if (line_length < 0) {
        *s = (struct str){0};
    } else {
        *s = (struct str){
            .ptr = iter->buffer,
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
    if (iter->buffer) free(iter->buffer);
}
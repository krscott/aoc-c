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
    for (size_t i = 0; i < lines->len; ++i) {
        strbuf_deinit(&lines->ptr[i]);
    }
    vec_deinit(linevec, lines);
}

NODISCARD char linevec_get(struct linevec const lines, size_t const row, size_t const col) {
    if (row >= lines.len) return '\0';
    struct strbuf const line = lines.ptr[row];
    if (col >= line.len) return '\0';
    return line.ptr[col];
}

ERRFN cli_file(FILE **const file, int argc, char **const argv) {
    assert(file);
    assert(argv);

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

ERRFN cli_file_lines(struct linevec *const lines, int const argc, char **const argv) {
    assert(lines);
    assert(argv);
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

        e = vec_push(linevec, lines, linebuf);
        if (e) goto error;
    }

error:
    fileiter_deinit(&iter);
    return e;
}

ERRFN fileiter_init_cli(struct fileiter *const iter, int const argc, char **const argv) {
    assert(iter);
    assert(argv);
    *iter = (struct fileiter){0};
    return cli_file(&iter->file, argc, argv);
}

ERRFN fileiter_line(struct str *const s, struct fileiter *const iter) {
    assert(s);
    assert(iter);
    ssize_t const line_length = getline(&iter->buffer, &iter->size, iter->file);
    if (errno) {
        log_err("getline failed: %s", strerror(errno));
        *s = (struct str){0};
        return ERR_ERRNO;
    } else if (line_length < 0) {
        *s = (struct str){0};
        return ERR_NONE;
    } else {
        *s = str_trim_whitespace((struct str){
            .ptr = iter->buffer,
            .len = (size_t)line_length,
        });
        str_log_dbg(*s);
        return OK;
    }
}

void fileiter_deinit(struct fileiter *const iter) {
    assert(iter);
    if (iter->file) fclose(iter->file);
    if (iter->buffer) free(iter->buffer);
}

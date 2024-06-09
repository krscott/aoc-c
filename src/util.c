#include "util.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern inline void cstrbuf_assert_valid(struct cstrbuf *s);

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
        case ERR_MEM:
            return "Out of Memory";
    }
}

struct str str_substr(struct str s, ssize_t start, ssize_t end) {
    assert(s.ptr);
    assert(end > start);
    assert(start >= 0 && start < s.len);
    assert(end > 0 && end <= s.len);
    return (struct str){
        .ptr = s.ptr + start,
        .len = end - start,
    };
}

char str_split(struct str *left, struct str *right, struct str input, char const *delims) {
    if (input.ptr == NULL) {
        if (left) *left = (struct str){0};
        if (right) *right = (struct str){0};
        return '\0';
    }

    for (ssize_t i = 0; i < input.len; ++i) {
        char *match = strchr(delims, input.ptr[i]);
        if (match) {
            if (left) {
                left->ptr = input.ptr;
                left->len = i;
            }
            if (right) {
                right->ptr = &input.ptr[i + 1];
                right->len = input.len - i - 1;
            }
            return *match;
        }
    }

    if (left) *left = input;
    if (right) *right = (struct str){0};
    return '\0';
}

enum err str_take_int(i32 *n, struct str *tail, struct str input) {
    assert(n);
    char *end;
    *n = strtol(input.ptr, &end, 10);
    if (errno) {
        log_err("strol failed: %s", strerror(errno));
        return ERR_ERRNO;
    }
    if (tail) {
        tail->ptr = end;
        tail->len = input.len - (end - input.ptr);
    }
    return OK;
}

struct str str_trim_whitespace(struct str s) {
    if (s.len <= 0) return s;
    while (s.len > 0 && isspace(s.ptr[s.len - 1])) {
        --s.len;
    }
    while (s.len > 0 && isspace(s.ptr[0])) {
        ++s.ptr;
        --s.len;
    };
    return s;
}

struct cstrbuf cstrbuf_from_owned_cstr(char *ptr) {
    if (!ptr) return (struct cstrbuf){0};

    ssize_t len = strlen(ptr);
    return (struct cstrbuf){
        .ptr = ptr,
        .len = len,
        .cap = len + 1,
    };
}

enum err cstrbuf_reserve(struct cstrbuf *s, ssize_t additional) {
    ssize_t const min_cap = 8;

    cstrbuf_assert_valid(s);
    assert(additional > 0);
    ssize_t total = s->len + 1 + additional;

    if (total > s->cap) {
        ssize_t new_cap = (s->cap >= min_cap) ? s->cap * 2 : min_cap;
        while (total > new_cap) {
            new_cap *= 2;
        }

        char *new_ptr = realloc(s->ptr, new_cap);
        if (!new_ptr) return ERR_MEM;

        s->ptr = new_ptr;
        s->cap = new_cap;
        assert(s->len < s->cap);
        s->ptr[s->len] = '\0';
    }

    return OK;
}

enum err cstrbuf_push(struct cstrbuf *s, char ch) {
    cstrbuf_assert_valid(s);
    assert(ch);

    enum err e = cstrbuf_reserve(s, 1);
    if (e) return e;

    s->ptr[s->len] = ch;
    ++s->len;
    s->ptr[s->len] = '\0';

    return OK;
}

void cstrbuf_deinit(struct cstrbuf *s) {
    if (s->ptr) free(s->ptr);
}

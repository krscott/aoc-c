#include "str.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "error.h"

extern inline void strbuf_assert_valid(struct strbuf *s);

NODISCARD struct str str_substr(struct str s, ssize_t start, ssize_t end) {
    assert(s.ptr);
    assert(end > start);
    assert(start >= 0 && start < s.len);
    assert(end > 0 && end <= s.len);
    return (struct str){
        .ptr = s.ptr + start,
        .len = end - start,
    };
}

struct str str_split(struct str *tail, struct str input, char const *delim) {
    assert(delim);

    if (input.ptr == NULL) {
        if (tail) *tail = (struct str){0};
        return (struct str){0};
    }

    ssize_t delim_len = strlen(delim);
    assert(delim_len > 0);

    for (ssize_t i = 0; i < input.len - delim_len + 1; ++i) {
        if (strncmp(&input.ptr[i], delim, delim_len) == 0) {
            *tail = (struct str){
                .ptr = &input.ptr[i + delim_len],
                .len = input.len - i - delim_len,
            };
            return (struct str){
                .ptr = input.ptr,
                .len = i,
            };
        }
    }

    *tail = (struct str){0};
    return input;
}

char str_split_any(struct str *head, struct str *tail, struct str input, char const *delims) {
    assert(delims);

    if (input.ptr == NULL) {
        if (head) *head = (struct str){0};
        if (tail) *tail = (struct str){0};
        return '\0';
    }

    for (ssize_t i = 0; i < input.len; ++i) {
        char *match = strchr(delims, input.ptr[i]);
        if (match) {
            if (head) {
                head->ptr = input.ptr;
                head->len = i;
            }
            if (tail) {
                tail->ptr = &input.ptr[i + 1];
                tail->len = input.len - i - 1;
            }
            return *match;
        }
    }

    if (head) *head = input;
    if (tail) *tail = (struct str){0};
    return '\0';
}

char str_shift(struct str *tail, struct str input) {
    assert(input.len >= 0);

    if (input.len <= 0) {
        if (tail) {
            tail->ptr = NULL;
            tail->len = 0;
        }
        return '\0';
    }

    if (tail) {
        tail->ptr = input.ptr + 1;
        tail->len = input.len - 1;
    }
    return input.ptr[0];
}

ERRFN str_take_int(i64 *n, struct str *tail, struct str input) {
    assert(n);

    if (input.len == 0) return ERR_NONE;

    char *end;
    errno = 0;
    *n = strtol(input.ptr, &end, 10);
    if (*n == 0 && end == input.ptr) {
        if (tail) *tail = input;
        return ERR_NONE;
    }
    if (errno) return err_trace(ERR_ERRNO);

    if (tail) {
        tail->ptr = end;
        tail->len = input.len - (end - input.ptr);
    }
    return OK;
}

NODISCARD struct str str_trim_whitespace(struct str s) {
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

NODISCARD struct strbuf strbuf_from_owned_cstr(char *ptr) {
    if (!ptr) return (struct strbuf){0};

    ssize_t len = strlen(ptr);
    return (struct strbuf){
        .ptr = ptr,
        .len = len,
        .cap = len + 1,
    };
}

ERRFN strbuf_init_copy_str(struct strbuf *s, struct str other) {
    assert(s);

    *s = (struct strbuf){0};
    if (other.len == 0) return OK;

    ssize_t const cap = other.len + 1;

    s->ptr = malloc(cap * sizeof(char));
    if (!s->ptr) return err_trace(ERR_MEM);

    strncpy(s->ptr, other.ptr, other.len);
    s->ptr[other.len] = '\0';
    s->len = other.len;
    s->cap = cap;

    return OK;
}

ERRFN strbuf_init_copy_cstr(struct strbuf *s, char const *ptr) {
    *s = (struct strbuf){0};
    if (!ptr) return OK;

    ssize_t const len = strlen(ptr);
    if (len == 0) return OK;

    ssize_t const cap = len + 1;

    s->ptr = malloc(cap * sizeof(char));
    if (!s->ptr) return err_trace(ERR_MEM);

    strcpy(s->ptr, ptr);
    s->len = len;
    s->cap = cap;

    return OK;
}

ERRFN strbuf_reserve(struct strbuf *s, ssize_t additional) {
    ssize_t const min_cap = 8;

    strbuf_assert_valid(s);
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

ERRFN strbuf_push(struct strbuf *s, char ch) {
    strbuf_assert_valid(s);
    assert(ch);

    enum err e = strbuf_reserve(s, 1);
    if (e) return e;

    s->ptr[s->len] = ch;
    ++s->len;
    s->ptr[s->len] = '\0';

    return OK;
}

void strbuf_deinit(struct strbuf *s) {
    if (s->ptr) free(s->ptr);
}

void str_println(struct str s) { printf("%.*s\n", (int)s.len, s.ptr); }

#include "str.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "error.h"
#include "log.h"

extern inline void strbuf_assert_valid(struct strbuf *s);
extern inline NODISCARD struct str strbuf_to_str(struct strbuf s);

NODISCARD struct str str_substr(struct str const s, size_t const start, size_t const end) {
    assert(s.ptr);
    assert(end > start);
    assert(start < s.len);
    assert(end > 0 && end <= s.len);
    return (struct str){
        .ptr = s.ptr + start,
        .len = end - start,
    };
}

struct str str_split(struct str *const tail, struct str const input, char const *delim) {
    // tail may be null
    assert(delim);

    if (input.ptr == NULL) {
        if (tail) *tail = (struct str){0};
        return (struct str){0};
    }

    size_t const delim_len = strlen(delim);
    assert(delim_len > 0);

    for (size_t i = 0; i < input.len - delim_len + 1; ++i) {
        if (strncmp(&input.ptr[i], delim, delim_len) == 0) {
            size_t const i_delim_offset = i + delim_len;
            assert(i_delim_offset <= input.len);
            *tail = (struct str){
                .ptr = &input.ptr[i_delim_offset],
                .len = input.len - i_delim_offset,
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

char str_split_any(
    struct str *const head,
    struct str *const tail,
    struct str const input,
    char const *const delims
) {
    // head and tail may be null
    assert(delims);

    if (input.ptr == NULL) {
        if (head) *head = (struct str){0};
        if (tail) *tail = (struct str){0};
        return '\0';
    }

    for (size_t i = 0; i < input.len; ++i) {
        char const *const match = strchr(delims, input.ptr[i]);
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

char str_shift(struct str *const tail, struct str const input) {
    // tail may be null
    if (input.len == 0) {
        if (tail) {
            tail->ptr = NULL;
            tail->len = 0;
        }
        return '\0';
    } else {
        if (tail) {
            tail->ptr = input.ptr + 1;
            tail->len = input.len - 1;
        }
        return input.ptr[0];
    }
}

ERRFN str_take_int(i64 *const n, struct str *const tail, struct str const input) {
    // tail may be null
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
        ssize_t const n_str_len = end - input.ptr;
        assert(0 < n_str_len && (size_t)n_str_len <= input.len);
        tail->ptr = end;
        tail->len = input.len - (size_t)n_str_len;
    }
    return OK;
}

NODISCARD struct str str_trim_whitespace(struct str s) {
    if (s.len == 0) return s;

    assert(s.ptr);
    while (s.len > 0 && isspace(s.ptr[s.len - 1])) {
        --s.len;
    }
    while (s.len > 0 && isspace(s.ptr[0])) {
        ++s.ptr;
        --s.len;
    };
    return s;
}

NODISCARD struct strbuf strbuf_from_owned_cstr(char *const ptr) {
    if (!ptr) return (struct strbuf){0};

    size_t const len = strlen(ptr);
    return (struct strbuf){
        .ptr = ptr,
        .len = len,
        .cap = len + 1,
    };
}

ERRFN strbuf_init_copy_str(struct strbuf *const s, struct str const other) {
    assert(s);

    *s = (struct strbuf){0};
    if (other.len == 0) return OK;

    size_t const cap = other.len + 1;

    s->ptr = malloc(cap * sizeof(char));
    if (!s->ptr) return err_trace(ERR_MEM);

    strncpy(s->ptr, other.ptr, other.len);
    s->ptr[other.len] = '\0';
    s->len = other.len;
    s->cap = cap;

    return OK;
}

ERRFN strbuf_init_copy_cstr(struct strbuf *const s, char const *const ptr) {
    if (!ptr) {
        *s = (struct strbuf){0};
        return OK;
    }

    size_t const len = strlen(ptr);
    if (len == 0) {
        *s = (struct strbuf){0};
        return OK;
    }

    size_t const cap = len + 1;

    s->ptr = malloc(cap * sizeof(char));
    if (!s->ptr) {
        *s = (struct strbuf){0};
        return err_trace(ERR_MEM);
    }

    strcpy(s->ptr, ptr);
    s->len = len;
    s->cap = cap;

    return OK;
}

ERRFN strbuf_reserve(struct strbuf *const s, size_t const additional) {
    strbuf_assert_valid(s);
    if (additional == 0) return OK;

    size_t const min_cap = 8;
    size_t total = s->len + 1 + additional;

    if (total > s->cap) {
        size_t new_cap = (s->cap >= min_cap) ? s->cap * 2 : min_cap;
        while (total > new_cap) {
            new_cap *= 2;
        }

        char *const new_ptr = realloc(s->ptr, new_cap);
        if (!new_ptr) return ERR_MEM;

        s->ptr = new_ptr;
        s->cap = new_cap;
        assert(s->len < s->cap);
        s->ptr[s->len] = '\0';
    }

    return OK;
}

ERRFN strbuf_push(struct strbuf *const s, char const ch) {
    strbuf_assert_valid(s);
    assert(ch);

    enum err const e = strbuf_reserve(s, 1);
    if (e) return e;

    s->ptr[s->len] = ch;
    ++s->len;
    s->ptr[s->len] = '\0';

    return OK;
}

ERRFN strbuf_replace(struct strbuf *const s, char const *const find, char const *const replace) {
    assert(s);
    assert(find);
    assert(replace);
    size_t const find_len = strlen(find);
    size_t const replace_len = strlen(replace);

    char const *const match = strstr(s->ptr, find);
    if (!match) return ERR_NONE;

    size_t const match_idx = (size_t)(match - s->ptr);

    if (replace_len > find_len) {
        size_t const diff_len = replace_len - find_len;
        enum err const e = strbuf_reserve(s, diff_len);
        if (e) return e;

        for (size_t i = s->len; i >= match_idx + find_len; --i) {
            s->ptr[i] = s->ptr[i - diff_len];
        }

        s->len += diff_len;
    } else if (replace_len < find_len) {
        size_t const diff_len = find_len - replace_len;
        assert(match_idx >= replace_len);
        for (size_t i = match_idx - replace_len; i < s->len; ++i) {
            s->ptr[i] = s->ptr[i + diff_len];
        }
        s->len -= diff_len;
    }

    for (size_t i = 0; i < replace_len; ++i) {
        s->ptr[match_idx + i] = replace[i];
    }

    return OK;
}

ERRFN strbuf_replace_all(struct strbuf *s, char const *find, char const *replace) {
    enum err e;
    for (;;) {
        e = strbuf_replace(s, find, replace);
        if (e == ERR_NONE) return OK;
        if (e) return e;
    }
}

void strbuf_deinit(struct strbuf *s) {
    if (s->ptr) free(s->ptr);
}

void str_println(struct str s) { printf("%.*s\n", (int)s.len, s.ptr); }

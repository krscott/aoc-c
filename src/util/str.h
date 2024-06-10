#ifndef STR_H
#define STR_H

#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "error.h"

// Fat pointer string view
struct str {
    char *ptr;
    ssize_t len;
};

/// Get substring from start to end (exclusive)
struct str str_substr(struct str s, ssize_t start, ssize_t end);
/// Split a str by delim. Returns matched delim, or '\0' if end of string
char str_split(struct str *left, struct str *right, struct str input, char const *delims);
/// Remove first char from intput, put result in tail
char str_shift(struct str *tail, struct str input);
/// Parse an integer, put remaining string in tail. If no number, returns ERR_NONE.
enum err str_take_int(i32 *n, struct str *tail, struct str input);
/// Trim whitespace
struct str str_trim_whitespace(struct str s);

// Owned, heap-allocated, null-terminated string buffer
struct cstrbuf {
    char *ptr;
    /// Length of string, excluding null byte
    ssize_t len;
    /// Capacity of pointer memory
    ssize_t cap;
};

inline void cstrbuf_assert_valid(struct cstrbuf *s) {
    assert(s);
    if (s->cap > 0) {
        assert(s->ptr != NULL);
        assert(s->len >= 0);
        assert(s->len < s->cap);
    } else {
        assert(s->ptr == NULL);
        assert(s->len == 0);
        assert(s->cap == 0);
    }
}

void cstrbuf_deinit(struct cstrbuf *s);
/// Move an owned null-terminated string into cstrbuf
struct cstrbuf cstrbuf_from_owned_cstr(char *ptr);
/// Create a cstrbuf by copying a cstr
enum err cstrbuf_init_copy_cstr(struct cstrbuf *s, char const *ptr);
/// Create a cstrbuf by copying a str
enum err cstrbuf_init_copy_str(struct cstrbuf *s, struct str other);
/// Reserve additional bytes beyond length
enum err cstrbuf_reserve(struct cstrbuf *s, ssize_t additional);
/// Append a character to the end of the buffer and move the null terminator
enum err cstrbuf_push(struct cstrbuf *s, char ch);
/// Get a str view
inline struct str cstrbuf_to_str(struct cstrbuf s) {
    return (struct str){
        .ptr = s.ptr,
        .len = s.len,
    };
}

#endif  // STR_H

#ifndef STR_H
#define STR_H

#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "error.h"

/// Fat pointer string view
struct str {
    char *ptr;
    ssize_t len;
};

/// Get substring from start to end (exclusive)
NODISCARD struct str str_substr(struct str s, ssize_t start, ssize_t end);
/// Split a str by delim. Return next split section.
struct str str_split(struct str *tail, struct str input, char const *delim);
/// Split a str by any char in delims. Returns matched char, or '\0' if end of string
char str_split_any(struct str *head, struct str *tail, struct str input, char const *delims);
/// Remove first char from intput, put result in tail
char str_shift(struct str *tail, struct str input);
/// Parse an integer, put remaining string in tail. If no number, returns ERR_NONE.
ERRFN str_take_int(i64 *n, struct str *tail, struct str input);
/// Trim whitespace
NODISCARD struct str str_trim_whitespace(struct str s);
/// Print str to stdout
void str_println(struct str s);
/// Print str to debug log
#define str_log_dbg(s) log_dbg("%.*s", (int)(s).len, (s).ptr)

/// Owned, heap-allocated, null-terminated string buffer
struct strbuf {
    char *ptr;
    /// Length of string, excluding null byte
    ssize_t len;
    /// Capacity of pointer memory
    ssize_t cap;
};

inline void strbuf_assert_valid(struct strbuf *s) {
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

void strbuf_deinit(struct strbuf *s);
/// Move an owned null-terminated string into cstrbuf
NODISCARD struct strbuf strbuf_from_owned_cstr(char *ptr);
/// Create a cstrbuf by copying a cstr
ERRFN strbuf_init_copy_cstr(struct strbuf *s, char const *ptr);
/// Create a cstrbuf by copying a str
ERRFN strbuf_init_copy_str(struct strbuf *s, struct str other);
/// Reserve additional bytes beyond length
ERRFN strbuf_reserve(struct strbuf *s, ssize_t additional);
/// Append a character to the end of the buffer and move the null terminator
ERRFN strbuf_push(struct strbuf *s, char ch);
/// Get a str view
inline NODISCARD struct str strbuf_to_str(struct strbuf s) {
    return (struct str){
        .ptr = s.ptr,
        .len = s.len,
    };
}
/// Replace `find` string with `replace` string. Returns `ERR_NONE` if no match.
ERRFN strbuf_replace(struct strbuf *s, char const *find, char const *replace);
/// Replace all instances of `find` string with `replace` string.
ERRFN strbuf_replace_all(struct strbuf *s, char const *find, char const *replace);

#endif  // STR_H

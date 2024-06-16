#ifndef FILEITER_H
#define FILEITER_H

#include "common.h"
#include "error.h"
#include "str.h"
#include "vec.h"

struct fileiter {
    FILE *file;
    size_t size;
    char *buffer;
};

vec_define_struct(linevec, struct strbuf);
void linevec_deinit(struct linevec *lines);
NODISCARD char linevec_get(struct linevec lines, size_t row, size_t col);

ERRFN cli_file(FILE **file, int argc, char *argv[]);
ERRFN cli_file_lines(struct linevec *lines, int argc, char *argv[]);
ERRFN fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]);
/// Read a line from fileiter and set `s` as a reference.
/// `s` is only valid until the next call to `fileiter_line()`.
/// if `s` is `NULL`, the line is still skipped.
ERRFN fileiter_line(struct str *s, struct fileiter *iter);
ERRFN fileiter_line_strbuf_init(struct strbuf *const sb, struct fileiter *const iter);
ERRFN fileiter_skip_blank_line(struct fileiter *iter);
ERRFN fileiter_delims(struct str *s, struct fileiter *iter, char const *delims);
void fileiter_deinit(struct fileiter *iter);

#endif  // FILEITER_H

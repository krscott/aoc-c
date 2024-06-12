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
NODISCARD char linevec_get(struct linevec lines, ssize_t row, ssize_t col);

ERRFN cli_file(FILE **file, int argc, char *argv[]);
ERRFN cli_file_lines(struct linevec *lines, int argc, char *argv[]);
ERRFN fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]);
ERRFN fileiter_line(struct str *s, struct fileiter *iter);
ERRFN fileiter_delims(struct str *s, struct fileiter *iter, char const *delims);
void fileiter_deinit(struct fileiter *iter);

#endif  // FILEITER_H

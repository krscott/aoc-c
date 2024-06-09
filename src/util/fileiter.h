#ifndef FILEITER_H
#define FILEITER_H

#include "error.h"
#include "str.h"
#include "vec.h"

struct fileiter {
    FILE *file;
    size_t size;
    char *buffer;
};

vec_define_struct(linevec, struct cstrbuf);
void linevec_deinit(struct linevec *lines);
char linevec_get(struct linevec lines, ssize_t row, ssize_t col);

enum err cli_file(FILE **file, int argc, char *argv[]);
enum err cli_file_lines(struct linevec *lines, int argc, char *argv[]);
enum err fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]);
enum err fileiter_line(struct str *s, struct fileiter *iter);
enum err fileiter_delims(struct str *s, struct fileiter *iter, char const *delims);
void fileiter_deinit(struct fileiter *iter);

#endif  // FILEITER_H

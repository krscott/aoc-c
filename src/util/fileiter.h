#ifndef FILEITER_H
#define FILEITER_H

#include "error.h"
#include "str.h"

struct fileiter {
    FILE *file;
    size_t size;
    char *buffer;
};

enum err cli_file(FILE **file, int argc, char *argv[]);
enum err fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]);
enum err fileiter_line(struct str *s, struct fileiter *iter);
enum err fileiter_delims(struct str *s, struct fileiter *iter, char const *delims);
void fileiter_deinit(struct fileiter *iter);

#endif  // FILEITER_H

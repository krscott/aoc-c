#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>

typedef int32_t i32;

enum err {
    OK = 0,
    ERR_CLI,
    ERR_FS,
    ERR_INPUT,
    ERR_ERRNO,
};

struct fileiter {
    FILE *file;
    size_t size;
    char *line;
};

struct str {
    char *ptr;
    ssize_t len;
};

enum err cli_file(FILE **file, int argc, char *argv[]);
enum err fileiter_init_cli(struct fileiter *iter, int argc, char *argv[]);
enum err fileiter_line(struct str *s, struct fileiter *iter);
void fileiter_deinit(struct fileiter *iter);

char const *err_string(enum err e);

struct str str_substr(struct str s, ssize_t start, ssize_t end);

#define countof(a) (sizeof(a) / sizeof(*(a)))

#define log(type, fmt, ...) \
    fprintf(stderr, __FILE__ ":%d " type " " fmt "\n", __LINE__, ##__VA_ARGS__)

#define log_err(...) log("ERROR", __VA_ARGS__)

#define panic(fmt, ...)            \
    do {                           \
        log("PANIC", __VA_ARGS__); \
        abort();                   \
    } while (0)

#define panic_on_err(e) panic("%s", error_string(e))

#endif  // UTIL_H

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
};

enum err cli(FILE **file, int argc, char *argv[]);
char const *error_string(enum err e);

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

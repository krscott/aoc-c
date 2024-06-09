#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

enum err {
    OK = 0,
    ERR_CLI,
    ERR_FS,
    ERR_INPUT,
    ERR_ERRNO,
    ERR_MEM,
};

char const *err_string(enum err e);

#define log(type, fmt, ...) \
    fprintf(stderr, __FILE__ ":%d " type " " fmt "\n", __LINE__, ##__VA_ARGS__)

#define log_err(...) log("ERROR", __VA_ARGS__)

#define panic(fmt, ...)            \
    do {                           \
        log("PANIC", __VA_ARGS__); \
        abort();                   \
    } while (0)

#define panic_on_err(e) panic("%s", error_string(e))

#endif  // ERROR_H

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef __clang__
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define log(type, fmt, ...) \
    fprintf(stderr, type " " __FILE__ ":%d " fmt "\n", __LINE__, ##__VA_ARGS__)

#define log_err(...) log("ERROR", __VA_ARGS__)

#ifndef LOG_DBG
#define LOG_DBG 0
#endif

#if LOG_DBG
#define log_dbg(...) log("DEBUG", __VA_ARGS__)
#else
#define log_dbg(...)
#endif

/* #define panic(fmt, ...)            \ */
/*     do {                           \ */
/*         log("PANIC", __VA_ARGS__); \ */
/*         abort();                   \ */
/*     } while (0) */

/* #define panic_on_err(e) panic("%s", err_string(e)) */

#endif  // LOG_H

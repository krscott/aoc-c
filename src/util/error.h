#ifndef ERROR_H
#define ERROR_H

#include "log.h"

enum err {
    OK = 0,
    ERR_NONE,
    ERR_CLI,
    ERR_FS,
    ERR_INPUT,
    ERR_ERRNO,
    ERR_MEM,
};

char const* err_string(enum err e);

#define err_trace(e) err_trace_info(e, __FILE_NAME__, __LINE__)
enum err err_trace_info(enum err e, char const* filename, int lineno);

#endif  // ERROR_H

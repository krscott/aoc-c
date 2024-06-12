#ifndef ERROR_H
#define ERROR_H

#include "common.h"  // IWYU pragma: export

enum err {
    OK = 0,
    ERR_NONE,
    ERR_CLI,
    ERR_FS,
    ERR_INPUT,
    ERR_ERRNO,
    ERR_MEM,
};

#define ERRFN NODISCARD enum err

NODISCARD char const* err_string(enum err e);

#define err_trace(e) err_trace_info(e, my_basename(__FILE__), __LINE__)
ERRFN err_trace_info(enum err e, char const* filename, int lineno);

/// Wrapper function for macros to force using error
ERRFN err_nodiscard(enum err e);

#endif  // ERROR_H

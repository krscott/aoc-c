#include "error.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

ERRFN err_nodiscard(enum err e) { return e; }

NODISCARD char const* err_string(enum err e) {
    switch (e) {
        case OK:
            return "OK";
        case ERR_NONE:
            return "None";
        case ERR_CLI:
            return "Command Line Argument Error";
        case ERR_FS:
            return "Filesystem Error";
        case ERR_INPUT:
            return "Invalid Input";
        case ERR_ERRNO:
            return strerror(errno);
        case ERR_MEM:
            return "Out of Memory";
    }
    assert(0);
    return "Unknown Error";
}

ERRFN err_trace_info(enum err e, char const* filename, int lineno) {
    if (e) fprintf(stderr, "ERROR %s:%d %s\n", filename, lineno, err_string(e));
    return e;
}

#include "error.h"

#include <errno.h>
#include <string.h>

char const *err_string(enum err e) {
    switch (e) {
        case OK:
            return "OK";
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
}

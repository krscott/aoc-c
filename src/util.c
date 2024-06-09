#include "util.h"

enum err cli(FILE **file, int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s INPUT_FILE\n", argv[0]);
        return ERR_CLI;
    }

    *file = fopen(argv[1], "r");
    if (*file == NULL) {
        fprintf(stderr, "Could not read file: %s\n", argv[1]);
        return ERR_FS;
    }

    return OK;
}

char const *error_string(enum err e) {
    switch (e) {
        case OK:
            return "OK";
        case ERR_CLI:
            return "Command Line Argument Error";
        case ERR_FS:
            return "Filesystem Error";
        case ERR_INPUT:
            return "Invalid Input";
    }
}

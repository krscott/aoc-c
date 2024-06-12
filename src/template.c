#include "util/fileiter.h"

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    i64 total = 0;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e == ERR_NONE) break;
        if (e) goto error;
    }

    printf("%ld\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

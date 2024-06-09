#include "util/fileiter.h"

int main(int argc, char *argv[]) {
    struct fileiter f;
    enum err e = fileiter_init_cli(&f, argc, argv);
    if (e) goto error;

    i32 total = 0;

    for (;;) {
        struct str line;
        e = fileiter_line(&line, &f);
        if (e) goto error;
        if (!line.ptr) break;
    }

    printf("%d\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

#include "day02shared.h"
#include "util/fileiter.h"

static enum err parse_line(i32 *total, struct str line) {
    struct linedata data = {0};
    enum err e = linedata_get(&data, line);
    if (e) goto error;

    *total += data.r * data.g * data.b;

error:
    return e;
}

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

        e = parse_line(&total, line);
        if (e) goto error;
    }

    printf("%d\n", total);

error:
    fileiter_deinit(&f);
    return e;
}

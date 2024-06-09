#ifndef DAY02SHARED_H
#define DAY02SHARED_H

#include "util/common.h"
#include "util/error.h"
#include "util/str.h"

struct linedata {
    i32 id;
    i32 r;
    i32 g;
    i32 b;
};

enum err linedata_get(struct linedata *data, struct str line);

#endif  // DAY02SHARED_H

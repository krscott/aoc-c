#include "vec.h"

#include <assert.h>
#include <stdlib.h>

#include "error.h"

vec_define_struct(vec__anyvec, void);

void vec__deinit(struct vec__anyvec *const vec, size_t const elem_size) {
    assert(vec);
    (void)elem_size;
    if (vec->ptr) free(vec->ptr);
}

ERRFN vec__reserve(struct vec__anyvec *const vec, size_t const elem_size, size_t const additional) {
    assert(vec);
    if (additional == 0) return OK;

    size_t const min_cap = 8;
    size_t const total = vec->len + additional;

    if (total > vec->cap) {
        size_t new_cap = (vec->cap >= min_cap) ? vec->cap * 2 : min_cap;
        while (total > new_cap) {
            new_cap *= 2;
        }

        char *const new_buf = realloc(vec->ptr, new_cap * elem_size);
        if (!new_buf) return err_trace(ERR_MEM);

        vec->ptr = new_buf;
        vec->cap = new_cap;
        assert(vec->len < vec->cap);
    }

    return OK;
}

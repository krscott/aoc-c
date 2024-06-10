#include "vec.h"

#include <assert.h>
#include <stdlib.h>

#include "error.h"

vec_define_struct(vec, void);

void vec__deinit(struct vec *vec, size_t elem_size) {
    (void)elem_size;
    if (vec->buf) free(vec->buf);
}

enum err vec__reserve(struct vec *vec, size_t elem_size, ssize_t additional) {
    ssize_t const min_cap = 8;

    assert(additional > 0);
    ssize_t total = vec->len + additional;

    if (total > vec->cap) {
        ssize_t new_cap = (vec->cap >= min_cap) ? vec->cap * 2 : min_cap;
        while (total > new_cap) {
            new_cap *= 2;
        }

        char *new_buf = realloc(vec->buf, new_cap * elem_size);
        if (!new_buf) return err_trace(ERR_MEM);

        vec->buf = new_buf;
        vec->cap = new_cap;
        assert(vec->len < vec->cap);
    }

    return OK;
}

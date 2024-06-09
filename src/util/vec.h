#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <sys/types.h>

#include "error.h"

/// Generic type-less vector
struct vec;

#define vec_define_struct(name, T) \
    struct name {                  \
        T *buf;                    \
        ssize_t len;               \
        ssize_t cap;               \
    }

#define vec_detype_(v) (struct vec *)(v), sizeof(*(v)->buf)

#define vec_deinit(v) vec_deinit_(vec_detype_(v))
void vec_deinit_(struct vec *vec, size_t elem_size);

#define vec_reserve(v, additional) vec_reserve_(vec_detype_(v), additional)
enum err vec_reserve_(struct vec *vec, size_t elem_size, ssize_t additional);

#define vec_push(v, elem) (vec_reserve(v, 1) || ((v)->buf[(v)->len++] = elem, OK))

#define vec_clear(v) ((v)->len = 0)

#endif  // VEC_H

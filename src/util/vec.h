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

#define vec__detype(v) (struct vec *)(v), sizeof(*(v)->buf)

#define vec_deinit(v) vec__deinit(vec__detype(v))
void vec__deinit(struct vec *vec, size_t elem_size);

#define vec_reserve(v, additional) vec__reserve(vec__detype(v), additional)
enum err vec__reserve(struct vec *vec, size_t elem_size, ssize_t additional);

#define vec_push(v, elem) (vec_reserve(v, 1) || ((v)->buf[(v)->len++] = elem, OK))

#define vec_clear(v) ((v)->len = 0)

#endif  // VEC_H

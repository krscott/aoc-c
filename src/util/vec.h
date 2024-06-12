#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <sys/types.h>

#include "error.h"

/// Generic type-less vector
struct vec__anyvec;

#define vec_define_struct(name, T) \
    struct name {                  \
        T *ptr;                    \
        ssize_t len;               \
        ssize_t cap;               \
    };                             \
    struct name##__typecheck {     \
        struct name *inner;        \
    };                             \
    struct name##_slice {          \
        T *ptr;                    \
        ssize_t len;               \
    }

// Assign to name##__typecheck for type safety.
// Casting to (struct name *) is redundant, but required for IDE support
// (i.e. Try removing the cast and attempt to refactor-rename the struct)
#define vec__assert_type(name, v) ((struct name *)(struct name##__typecheck){.inner = (v)}.inner)

#define vec__destructure(name, v) (struct vec__anyvec *)(v), sizeof(*vec__assert_type(name, v)->ptr)

#define vec_deinit(name, v) vec__deinit(vec__destructure(name, v))
void vec__deinit(struct vec__anyvec *vec, size_t elem_size);

#define vec_reserve(name, v, additional) vec__reserve(vec__destructure(name, v), additional)
ERRFN vec__reserve(struct vec__anyvec *vec, size_t elem_size, ssize_t additional);

#define vec_push(name, v, elem) \
    err_nodiscard((vec_reserve(name, v, 1) || ((v)->ptr[(v)->len++] = elem, OK)))

#define vec_clear(name, v) ((v)->len = 0)

#define vec_slice(name, v)                     \
    ((struct name##_slice){                    \
        .ptr = vec__assert_type(name, v)->ptr, \
        .len = (v)->len,                       \
    })

#endif  // VEC_H

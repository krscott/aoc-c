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
        size_t len;                \
        size_t cap;                \
    }

#define span_define_struct(name, T) \
    struct name {                   \
        T *ptr;                     \
        size_t len;                 \
    }

// Struct type check macro
// Casting to (struct name *) is redundant, but required for IDE support
// (i.e. Try removing the cast and attempt to refactor-rename the struct)
#define vec__assert_type(S, v) ((S *)(struct { S *inner; }){.inner = (v)}.inner)

#define vec__destructure(name, v) \
    (struct vec__anyvec *)(v), sizeof(*vec__assert_type(struct name, (v))->ptr)

#define vec_deinit(name, v) vec__deinit(vec__destructure(name, (v)))
void vec__deinit(struct vec__anyvec *vec, size_t elem_size);

#define vec_reserve(name, v, additional) vec__reserve(vec__destructure(name, (v)), additional)
ERRFN vec__reserve(struct vec__anyvec *vec, size_t elem_size, size_t additional);

#define vec_push(name, v, elem) \
    err_nodiscard((vec_reserve(name, (v), 1) || ((v)->ptr[(v)->len++] = elem, OK)))

#define vec_clear(name, v) (vec__assert_type(struct name, (v))->len = 0)

#define vec_to_span(vec_name, span_name, v)                 \
    ((struct span_name){                                    \
        .ptr = vec__assert_type(struct vec_name, (v))->ptr, \
        .len = (v)->len,                                    \
    })

#define vec_sort(name, v, compar) vec__sort(vec__destructure(name, (v)), (compar))
void vec__sort(
    struct vec__anyvec *const vec,
    size_t const elem_size,
    int (*compar)(void const *, void const *)
);

#endif  // VEC_H

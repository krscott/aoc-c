#ifndef MODAD_TEMPLATE_H
#define MODAD_TEMPLATE_H

#include "sys/types.h"
#define VEC_INIT_CAP 8

// Note: This extra indirection is required for evaluating #define'd types like 'bool'
#define TYPE_TEMPLATE(name, tag) name##__##tag

#endif  // MODAD_TEMPLATE_H

#ifndef T
// #error "Missing: #define T <type>"
#define T int
#endif
#ifndef Tid
// #error "Missing: #define Tid <unique-name>"
#define Tid int
#endif

#define vec(Tid) TYPE_TEMPLATE(vec, Tid)
struct vec(Tid) {
    T* buf;
    ssize_t len;
    ssize_t cap;
};

#define slice(Tid) TYPE_TEMPLATE(slice, Tid)
struct slice(Tid) {
    T const* ptr;
    ssize_t len;
};

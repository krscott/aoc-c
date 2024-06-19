#ifndef BTREE_H
#define BTREE_H

#include "error.h"

typedef int (*btree_compare_fn_type)(void const *, void const *, void *);

#define btree_define_struct(name, T) \
    struct name {                    \
        T *elements;                 \
        struct name *links;          \
        size_t len;                  \
    }

struct btree__anytree;

#define btree_args(name, tree) \
    ((struct btree__anytree *)assert_type(struct name *, (tree))), (name##_degree)

#define btree_insert(name, tree, ...)                              \
    (btree__pre_insert(btree_args(name, tree)) ||                  \
     (/**/                                                         \
      ((tree)->elements[(tree)->len++] = (__VA_ARGS__)),           \
      btree__post_insert(btree_args(name, tree), (name##_compare)) \
     ))
ERRFN btree__pre_insert(struct btree__anytree *tree, size_t degree);
ERRFN btree__post_insert(struct btree__anytree *tree, size_t degree, btree_compare_fn_type compare);

#define btree_get(name, node, tree, key) \
    btree__get((node), btree_args(name, (tree)), (key), (name##_compare))
ERRFN btree__get(
    void *node,
    struct btree__anytree const *tree,
    size_t degree,
    void const *key,
    btree_compare_fn_type compare
);

#define btree_deinit(name, tree) btree__deinit(btree_args(name, tree))
void btree__deinit(struct btree__anytree *tree, size_t degree);

#endif  // BTREE_H
